#include "ISurface.h"
#include "Vector.h"
#include "Matrix.h"

struct ParametricInterval {
    ivec2 Divisions;
    vec2 UpperBound;
    vec2 TextureCount;
};

class ParametricSurface : public ISurface {
public:
    int GetVertexCount() const;
    int GetLineIndexCount() const;
    int GetTriangleIndexCount() const;
    void GenerateVertices(vector<float>& vertices, unsigned char flags) const;
    void GenerateLineIndices(vector<unsigned short>& indices) const;
    void GenerateTriangleIndices(vector<unsigned short>& indices) const;
protected:
    void SetInterval(const ParametricInterval& interval);
    virtual vec3 Evaluate(const vec2& domain) const = 0;
    virtual bool InvertNormal(const vec2& domain) const { return false; }
private:
    vec2 ComputeDomain(float i, float j) const;
    ivec2 m_slices;
    ivec2 m_divisions;
    vec2 m_upperBound;
    vec2 m_textureCount;
};

inline void ParametricSurface::SetInterval(const ParametricInterval& interval)
{
    m_divisions = interval.Divisions;
    m_upperBound = interval.UpperBound;
    m_textureCount = interval.TextureCount;
    m_slices = m_divisions - ivec2(1, 1);
}

inline int ParametricSurface::GetVertexCount() const
{
    return m_divisions.x * m_divisions.y;
}

inline int ParametricSurface::GetLineIndexCount() const
{
    return 4 * m_slices.x * m_slices.x;
}

inline int ParametricSurface::GetTriangleIndexCount() const
{
    return 6 * m_slices.x * m_slices.y;
}

inline vec2 ParametricSurface::ComputeDomain(float x, float y) const
{
    return vec2(x * m_upperBound.x / m_slices.x, y * m_upperBound.y / m_slices.y);
}

inline void
ParametricSurface::GenerateVertices(vector<float>& vertices,
                                    unsigned char flags) const
{
    int floatsPerVertex = 3;
    if (flags & VertexFlagsNormals)
        floatsPerVertex += 3;
    if (flags & VertexFlagsTexCoords)
        floatsPerVertex += 2;
	
    vertices.resize(GetVertexCount() * floatsPerVertex);
    float* attribute = &vertices[0];
	
    for (int j = 0; j < m_divisions.y; j++) {
        for (int i = 0; i < m_divisions.x; i++) {
			
            // Compute Position
            vec2 domain = ComputeDomain(i, j);
            vec3 range = Evaluate(domain);
            attribute = range.Write(attribute);
			
            // Compute Normal
            if (flags & VertexFlagsNormals) {
                float s = i, t = j;
				
                // Nudge the point if the normal is indeterminate.
                if (i == 0) s += 0.01f;
                if (i == m_divisions.x - 1) s -= 0.01f;
                if (j == 0) t += 0.01f;
                if (j == m_divisions.y - 1) t -= 0.01f;
                
                // Compute the tangents and their cross product.
                vec3 p = Evaluate(ComputeDomain(s, t));
                vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
                vec3 v = Evaluate(ComputeDomain(s, t + 0.01f)) - p;
                vec3 normal = u.Cross(v).Normalized();
                if (InvertNormal(domain))
                    normal = -normal;
                attribute = normal.Write(attribute);
            }
            
            // Compute Texture Coordinates
            if (flags & VertexFlagsTexCoords) {
                float s = m_textureCount.x * i / m_slices.x;
                float t = m_textureCount.y * j / m_slices.y;
                attribute = vec2(s, t).Write(attribute);
            }
        }
    }
}

inline void ParametricSurface::GenerateLineIndices(vector<unsigned short>& indices) const
{
    indices.resize(GetLineIndexCount());
    vector<unsigned short>::iterator index = indices.begin();
    for (int j = 0, vertex = 0; j < m_slices.y; j++) {
        for (int i = 0; i < m_slices.x; i++) {
            int next = (i + 1) % m_divisions.x;
            *index++ = vertex + i;
            *index++ = vertex + next;
            *index++ = vertex + i;
            *index++ = vertex + i + m_divisions.x;
        }
        vertex += m_divisions.x;
    }
}

inline void
ParametricSurface::GenerateTriangleIndices(vector<unsigned short>& indices) const
{
    indices.resize(GetTriangleIndexCount());
    vector<unsigned short>::iterator index = indices.begin();
    for (int j = 0, vertex = 0; j < m_slices.y; j++) {
        for (int i = 0; i < m_slices.x; i++) {
            int next = (i + 1) % m_divisions.x;
            *index++ = vertex + i;
            *index++ = vertex + next;
            *index++ = vertex + i + m_divisions.x;
            *index++ = vertex + next;
            *index++ = vertex + next + m_divisions.x;
            *index++ = vertex + i + m_divisions.x;
        }
        vertex += m_divisions.x;
    }
}

class Quad : public ParametricSurface {
public:
    Quad(float width, float height) : m_size(width, height)
    {
        ParametricInterval interval = { ivec2(2, 2), vec2(1, 1), vec2(1, 1) };
        SetInterval(interval);
    }
    vec3 Evaluate(const vec2& domain) const
    {
        float x = (domain.x - 0.5) * m_size.x;
        float y = (domain.y - 0.5) * m_size.y;
        float z = 0;
        return vec3(x, -y, z);
    }
private:
    vec2 m_size;
};

class Sphere : public ParametricSurface {
public:
    Sphere(float radius) : m_radius(radius)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(Pi, TwoPi), vec2(40, 70) };
        SetInterval(interval);
    }
    vec3 Evaluate(const vec2& domain) const
    {
        float u = domain.x, v = domain.y;
        float x = m_radius * sin(u) * cos(v);
        float y = m_radius * cos(u);
        float z = m_radius * -sin(u) * sin(v);
        return vec3(-y, x, z);
    }
private:
    float m_radius;
};

class Torus : public ParametricSurface {
public:
    Torus(float majorRadius, float minorRadius) :
	m_majorRadius(majorRadius),
	m_minorRadius(minorRadius)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(TwoPi, TwoPi), vec2(40, 10) };
        SetInterval(interval);
    }
    vec3 Evaluate(const vec2& domain) const
    {
        const float major = m_majorRadius;
        const float minor = m_minorRadius;
        float u = domain.x, v = domain.y;
        float x = (major + minor * cos(v)) * cos(u);
        float y = (major + minor * cos(v)) * sin(u);
        float z = minor * sin(v);
        return vec3(x, y, z);
    }
private:
    float m_majorRadius;
    float m_minorRadius;
};

class TrefoilKnot : public ParametricSurface {
public:
    TrefoilKnot(float scale) : m_scale(scale)
    {
        ParametricInterval interval = { ivec2(60, 15), vec2(TwoPi, TwoPi), vec2(100, 8) };
        SetInterval(interval);
    }
    vec3 Evaluate(const vec2& domain) const
    {
        const float a = 0.5f;
        const float b = 0.3f;
        const float c = 0.5f;
        const float d = 0.1f;
        float u = (TwoPi - domain.x) * 2;
        float v = domain.y;
        
        float r = a + b * cos(1.5f * u);
        float x = r * cos(u);
        float y = r * sin(u);
        float z = c * sin(1.5f * u);
        
        vec3 dv;
        dv.x = -1.5f * b * sin(1.5f * u) * cos(u) -
		(a + b * cos(1.5f * u)) * sin(u);
        dv.y = -1.5f * b * sin(1.5f * u) * sin(u) +
		(a + b * cos(1.5f * u)) * cos(u);
        dv.z = 1.5f * c * cos(1.5f * u);
        
        vec3 q = dv.Normalized();
        vec3 qvn = vec3(q.y, -q.x, 0).Normalized();
        vec3 ww = q.Cross(qvn);
        
        vec3 range;
        range.x = x + d * (qvn.x * cos(v) + ww.x * sin(v));
        range.y = y + d * (qvn.y * cos(v) + ww.y * sin(v));
        range.z = z + d * ww.z * sin(v);
        return range * m_scale;
    }
private:
    float m_scale;
};

class MobiusStrip : public ParametricSurface {
public:
    MobiusStrip(float scale) : m_scale(scale)
    {
        ParametricInterval interval = { ivec2(40, 20), vec2(TwoPi, TwoPi), vec2(40, 15) };
        SetInterval(interval);
    }
    vec3 Evaluate(const vec2& domain) const
    {
        float u = domain.x;
        float t = domain.y;
        float major = 1.25;
        float a = 0.125f;
        float b = 0.5f;
        float phi = u / 2;
        
        // General equation for an ellipse where phi is the angle
        // between the major axis and the X axis.
        float x = a * cos(t) * cos(phi) - b * sin(t) * sin(phi);
        float y = a * cos(t) * sin(phi) + b * sin(t) * cos(phi);
		
        // Sweep the ellipse along a circle, like a torus.
        vec3 range;
        range.x = (major + x) * cos(u);
        range.y = (major + x) * sin(u);
        range.z = y;
        return range * m_scale;
    }
private:
    float m_scale;
};

class KleinBottle : public ParametricSurface {
public:
    KleinBottle(float scale) : m_scale(scale)
    {
        ParametricInterval interval = { ivec2(20, 20), vec2(TwoPi, TwoPi), vec2(15, 50) };
        SetInterval(interval);
    }
    vec3 Evaluate(const vec2& domain) const
    {
        float v = 1 - domain.x;
        float u = domain.y;
        
        float x0 = 3 * cos(u) * (1 + sin(u)) +
		(2 * (1 - cos(u) / 2)) * cos(u) * cos(v);
        
        float y0  = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);
        
        float x1 = 3 * cos(u) * (1 + sin(u)) +
		(2 * (1 - cos(u) / 2)) * cos(v + Pi);
        
        float y1 = 8 * sin(u);
        
        vec3 range;
        range.x = u < Pi ? x0 : x1;
        range.y = u < Pi ? -y0 : -y1;
        range.z = (-2 * (1 - cos(u) / 2)) * sin(v);
        return range * m_scale;
    }
    bool InvertNormal(const vec2& domain) const
    {
        return domain.y > 3 * Pi / 2;
    }
private:
    float m_scale;
};
