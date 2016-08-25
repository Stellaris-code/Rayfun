#version 330

#extension GL_ARB_texture_rectangle : enable

#define MAX_RAY_STEPS 128

#define SIDE_NORTH 0
#define SIDE_SOUTH 1
#define SIDE_EAST 2
#define SIDE_WEST 3

uniform vec2 u_resolution;
uniform vec2 u_pos = vec2(7.5, 7.5);
uniform vec2 u_dir = vec2(0, 1);
uniform ivec2 u_mapSize;
uniform int u_shit;
uniform samplerBuffer u_map;
uniform sampler2DArray u_textures;

struct Tile
{
    bool isWall;
    uvec4 textures;
};
        
struct RaycastResult
{
        int side;
        bool hit;
        Tile tileHit;
        vec2 hitPos;
        ivec2 mapPos;
        float distance;
        float perpDistance;
};
    
uvec4 texAt(ivec2 mapPos)
{
    uvec4 texel = uvec4(texelFetch(u_map, mapPos.x + mapPos.y * u_mapSize.x));
    //uvec4 texel = uvec4(2, 2, 2, 2);
    return texel;
} 

void setPixelIntensity(inout vec4 color, float dist)
{
    const int multiplier = 10;
    float intensity = 0.5 / dist * multiplier;
    intensity = clamp(intensity, 0.0, 1.0);

    color.rgb *= vec3(intensity);
}

void setBrightness(inout vec4 color, float brightness)
{
    color.rgb *= vec3(brightness);
}
     
Tile tileAt(ivec2 mapPos)
{
    uvec4 textures = texAt(mapPos);
    Tile tile;
    tile.textures = textures;
    tile.isWall = any(greaterThan(textures, uvec4(0)));

    return tile;
}


RaycastResult raycast(vec2 t_begin, vec2 t_dir)
{
    vec2 rayPos = t_begin;
    vec2 rayDir = t_dir;

    ivec2 mapPos = ivec2(rayPos);
    
    vec2 deltaDist = abs(vec2(length(rayDir)) / rayDir);
    
    ivec2 rayStep = ivec2(sign(rayDir));

    vec2 sideDist = (sign(rayDir) * (vec2(mapPos) - rayPos) + (sign(rayDir) * 0.5) + 0.5) * deltaDist;

    float perpWallDist;
    
    int wallColor;
    bool hit = false; //was there a wall hit?
    int side = SIDE_NORTH;
    
    //perform DDA
    /* number specifies how far will the ray be checked against walls */
    bvec2 mask;
    for (int i = 0; i < length(u_mapSize); i++)
    {
        mask.x = sideDist.x < sideDist.y;
        mask.y = !mask.x;

        side = int(mask.x) * int(rayDir.x > 0) + int(mask.y) * (int(rayDir.y > 0) + 2);

        sideDist += vec2(mask) * deltaDist;
        mapPos += ivec2(mask) * rayStep;

        if (tileAt(mapPos).isWall)
        {
            hit = true;
            wallColor = 1;
            break;
        }
    } 

    vec2 perpWallDistVec = abs((vec2(mapPos) - rayPos + (vec2(1.0) - vec2(rayStep)) / vec2(2.0)) / rayDir);
    perpWallDistVec *= vec2(mask);

    perpWallDist = perpWallDistVec.x + perpWallDistVec.y;


    //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)

    float wallX; //where exactly the wall was hit
    vec2 wallXVec = (t_begin + vec2(perpWallDist) * t_dir) * vec2(not(mask));
    wallX = wallXVec.x + wallXVec.y;
    wallX -= floor(wallX);
    wallX = 1 - wallX;
    
    vec2 hitPos;
    {
        vec2 offset;
        offset.x = wallX * int(!mask.x);
        offset.y = wallX * int(mask.x);
        hitPos = mapPos + offset;
    }

    RaycastResult result;
    result.perpDistance = perpWallDist;
    result.distance = distance(t_begin, hitPos);
    result.side = side;
    result.tileHit = tileAt(mapPos);
    result.mapPos = mapPos;
    result.hitPos = hitPos;
    result.hit = hit;

    return result;
}

void floorCasting(int j, vec2 pos, RaycastResult result, float wallX, float drawEnd)
{
    vec2 floorWall;
    {
        vec2 toAdd;
        toAdd.x = 1 * int(result.side != SIDE_SOUTH) - int(result.side > SIDE_SOUTH) * wallX;
        toAdd.y = 1 * int(result.side != SIDE_EAST) - int(result.side < SIDE_EAST) * wallX;
        floorWall += result.mapPos + toAdd;
    }

    if (result.side == SIDE_SOUTH)
    {
        floorWall.x = result.mapPos.x;
        floorWall.y = result.mapPos.y + (1 - wallX);
    }
    else if (result.side == SIDE_NORTH)
    {
        floorWall.x = result.mapPos.x + 1;
        floorWall.y = result.mapPos.y + (1 - wallX);
    }
    else if (result.side == SIDE_WEST)
    {
        floorWall.x = result.mapPos.x + (1 - wallX);
        floorWall.y = result.mapPos.y;
    }
    else if (result.side == SIDE_EAST)
    {
        floorWall.x = result.mapPos.x + (1 - wallX);
        floorWall.y = result.mapPos.y + 1;
    }

    float currentDist = u_resolution.y / (2.0 * j - u_resolution.y );
    float weight = currentDist / result.perpDistance *
                (min(u_resolution.x, u_resolution.y)
                / max(u_resolution.x, u_resolution.y));
    vec2 floorPos = mix(pos, floorWall, weight);

    vec2 coord = floorPos - floor(floorPos);

    vec4 texel;
    texel = texture(u_textures, vec3(coord.xy, 1));
    setBrightness(texel, 0.8);

    if (j > u_resolution.y / 2)
    {
        setBrightness(texel, 0.6);
    }

    setPixelIntensity(texel, currentDist);

    gl_FragColor = texel;
}

void main()
{
/*    gl_FragColor.a = 1;

    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    
    vec2 camPos = u_pos;
    vec2 camDir = u_dir;

    int i = int(gl_FragCoord.x);
    int j = int(gl_FragCoord.y);

    mat2 rotationMatrix = mat2(0, -1, 1, 0);

    vec2 planeDir = camDir * rotationMatrix;
    float camX = 2.0 * uv.x - 1.0;

    vec2 rayPos = camPos;
    vec2 rayDir = camDir + planeDir * camX;
    

    RaycastResult result = raycast(rayPos, rayDir);

    gl_FragDepth = result.perpDistance;
    
    //Calculate height of line to draw on screen
    float lineHeight = (u_resolution.y / result.perpDistance) *
                (min(u_resolution.x, u_resolution.y)
                / max(u_resolution.x, u_resolution.y));

    //calculate lowest and highest pixel to fill in current stripe
    float drawStart = -lineHeight / 2.0 + u_resolution.y / 2.0;
    float drawEnd = lineHeight / 2.0 + u_resolution.y / 2.0;

    float wallX = result.side == SIDE_NORTH || result.side == SIDE_SOUTH ?
                result.hitPos.y - floor(result.hitPos.y) : result.hitPos.x - floor(result.hitPos.x);

    if (result.hit && j >= drawStart && j < drawEnd)
    {

        float texX = wallX;

        if (((result.side == SIDE_NORTH || result.side == SIDE_SOUTH) && rayDir.x > 0) ||
                        ((result.side == SIDE_EAST || result.side == SIDE_WEST) && rayDir.y < 0))
        {
            texX = texX - wallX*2;
        }

        const float brightness = 0.8;

        float texY = (j - drawStart) / (drawEnd - drawStart);
        vec4 color;

        vec2 coords = vec2(texX, texY);
        coords = coords - floor(coords);

        color = texture(u_textures, vec3(coords.xy, float(result.tileHit.textures[result.side]) - 1));

        setBrightness(color, brightness);
        setPixelIntensity(color, result.distance);

        if (color.a < 1.0)
        {
            vec4 oldColor = color;
            setPixelIntensity(oldColor, result.distance);

            if (result.side == SIDE_EAST || result.side == SIDE_WEST)
            {
                color.rgb /= 2.0;
            }

            vec4 mixedColor = mix(oldColor, color, color.a);
            mixedColor.a = 1.0;

            gl_FragColor = mixedColor;
        }
        else
        {
            if (result.side == SIDE_EAST || result.side == SIDE_WEST)
            {
                color.rgb /= 2.0;
            }
            color.a = 1.0;

            gl_FragColor = color;
        }

    }
    else if (j < drawStart)
    {
        floorCasting(int(u_resolution.y - j), rayPos, result, wallX, drawEnd);
    }
    else if (j > drawEnd)
    {
        floorCasting(int(j), rayPos, result, wallX, drawEnd);
    }

    if (result.side == SIDE_SOUTH)
    {
        
        //gl_FragColor = mix(gl_FragColor, vec4(0.75), 0.5);
    }
*/
    gl_FragColor = texture(u_textures, vec3(gl_FragCoord.xy / u_resolution.xy, 1));

    if (textureSize(u_textures, 0).z == 0)
    {
        gl_FragColor = vec4(1);
    }

    if (u_shit == 3)
    {
     //   gl_FragColor = vec4(0, 0, 1, 1);
    }

}