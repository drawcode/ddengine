//
//  Shader.vsh
//  ddengine
//
//  Created by drawcode on 4/23/10.
//  Copyright drawk  / drawcode / drawlabs 2010. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

uniform float translate;

void main()
{
    gl_Position = position;
    gl_Position.y += sin(translate) / 2.0;

    colorVarying = color;
}
