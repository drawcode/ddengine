//
//  Shader.fsh
//  ddengine
//
//  Created by drawcode on 4/23/10.
//  Copyright drawk  / drawcode / drawlabs 2010. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
