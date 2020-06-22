#include "ILI9163C_TFT.h"
#include "majda_gl.h" // contains glm!
#include "chunk_builder.h"


#define __CS  16
#define __RS  5
#define __DC  4

ILI9163C_TFT tft = ILI9163C_TFT(__CS, __RS, __DC);


int8_t z_buff[128][128] = { { 0 } };

void clear_z_buff()
{
  for(byte x = 0; x < 128; x++)
    for(byte y = 0; y < 128; y++)
      z_buff[x][y] = -127;
}

/*
uint8_t z_buff[128][128] = { { 0 } };

void clear_z_buff(uint8_t val = 0)
{
  for(byte x = 0; x < 128; x++)
    for(byte y = 0; y < 128; y++)
      z_buff[x][y] = val;
}

void process_z_buff()
{
  for(byte x = 0; x < 128; x++)
    for(byte y = 0; y < 128; y++)
    {
      if(z_buff[x][y] == 0)
        continue;
      
      if((z_buff[x][y] & 1 << 0) > 0)
      {
        z_buff[x][y] &= ~(1 << 0);
      }
      else
      {
        tft.set_pixel(x, y, BLACK);
        z_buff[x][y] = 0;
      }
    }
      
}
*/




//const int mesh_len = 12;
//triangle cube_mesh[mesh_len];

Camera camera = Camera(vec3f(0, -2, -4));
mat4f mat_proj;

triangle chunk_mesh[CHUNK_MESH_SIZE]; // yh limited
MeshEntity chunk1 = MeshEntity(chunk_mesh, 0, mat4f::identity());


ChunkBuilder cb;

void load() {

  
  // projection init
  float f_near = 0.1f;
  float f_far = 1000.0f;
  float fov = 90.0f * 3.14159f / 180.0f;
  float aspect = (float)tft.WIDTH / (float)tft.HEIGHT;

  mat_proj = mat4f::projection(fov, f_near, f_far, aspect);



  // camera init
  camera.build_view();


  Chunk c;
  c.fill(0);
  c.data[0][0][0] = c.data[1][0][0] = c.data[1][0][1] = c.data[2][0][2] = c.data[3][0][3] = c.data[0][1][0] = 1;

  chunk1 = cb.build_mesh(&c, &chunk_mesh[0]);

  if(chunk1.mesh_len > CHUNK_MESH_SIZE)
    chunk1.mesh_len = CHUNK_MESH_SIZE;
}


void render_entity(const MeshEntity& entity)
{
  triangle t;

  vec3f l1, l2, normal, to_camera;
  float dot_camera, lum;

  unsigned int color;

  mat4f world_mat = entity.model_mat * camera.view;

  for (int ti = 0; ti < entity.mesh_len; ti++)
  {
    t = entity.mesh[ti];

    // transformation
    t.p[0] = mat4f::mult_vec3f(t.p[0], world_mat);
    t.p[1] = mat4f::mult_vec3f(t.p[1], world_mat);
    t.p[2] = mat4f::mult_vec3f(t.p[2], world_mat);
    
    
    // normal calculation
    l1 = t.p[1] - t.p[0];
    l2 = t.p[2] - t.p[0];
    
    normal = vec3f::cross(l1, l2);
    normal.normalize();

    to_camera = t.p[0];
    to_camera.normalize();
    
    dot_camera = vec3f::dot(to_camera, normal);
    if(dot_camera > 0.0f) 
    {
      continue;
    }

    for(byte v = 0; v < 3; v++)
    {
      // projection
      t.p[v] = mat4f::mult_vec3f(t.p[v], mat_proj);

      // scale
      t.p[v].x += 1.0f;
      t.p[v].y += 1.0f;

      t.p[v].x *= 0.5f * (float)tft.WIDTH;
      t.p[v].y *= 0.5f * (float)tft.HEIGHT;
    }

    if(t.p[0].z > 1 || t.p[1].z > 1 || t.p[2].z > 1) // drawing behind camera
      continue;
    

    lum = min(dot_camera*-0.85f + 0.15f, 1.0f);
    
    color = lum * 31;   // r (31)
    color = (color << 6) + lum * 63; // g (63)
    color = (color << 5) + lum * 31; // b (31)
 
    
    //tft.draw_triangle_buff((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color, z_buff, (color & 0xfe));
    //tft.draw_triangle((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color);
    tft.draw_triangle_z((short)t.p[0].x, (short)t.p[0].y, (short)t.p[0].z, (short)t.p[1].x, (short)t.p[1].y,(short)t.p[1].z, (short)t.p[2].x, (short)t.p[2].y, (short)t.p[2].z, color, z_buff);
  }
}

void render_loop() {

}



#define __B1 0
#define __B2 2
#define __B3 12

bool render;

void setup()
{    
  pinMode(__B1, INPUT_PULLUP);
  pinMode(__B2, INPUT_PULLUP);
  
  tft.start();

  cb.init_data();
  
  load();
  render = true;


  tft.fill_screen(BLACK);
  pinMode(__B3, INPUT_PULLUP);
}



float mov;

void loop() {

  mov = 0;
  if(digitalRead(__B1) != HIGH)
    mov += 0.5f;

  if(digitalRead(__B2) != HIGH)
    mov -= 0.5f;

  if(mov != 0)
  {
    if(digitalRead(__B3) != HIGH)
    {
      camera.yaw -= mov;
    }
    else                                               
    {
      //auto camera_step = camera.strafe() * mov;
      //camera.pos = camera.pos - camera_step;
      camera.pos.x -= mov;
      camera.look_at.x -= mov;
    }

    camera.build_view();
    render = true;
  }


  
  
  
  if(render)
  {
    tft.fill_screen(BLACK);
    clear_z_buff();

    //render_loop();

    render_entity(chunk1);
    
    //process_z_buff();
   
    render = false;
  } 

  //delay(14);
}
