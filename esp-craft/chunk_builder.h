#include "majda_gl.h"
#include "chunk.h"

class ChunkBuilder
{
private:
  triangle m_front1;// = triangle(1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f);
  triangle m_front2;// = triangle(1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f);

  triangle m_back1;// = triangle(0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f);
  triangle m_back2;// = triangle(0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f);

  triangle m_left1;// = triangle(0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f);
  triangle m_left2;// = triangle(0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f);

  triangle m_right1;// = triangle(1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f);
  triangle m_right2;// = triangle(1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f);

  triangle m_top1;// = triangle(0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f);
  triangle m_top2;// = triangle(0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f);

  triangle m_bottom1;// = triangle(1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f);
  triangle m_bottom2;// = triangle(1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f);

  void add_mesh_face(Chunk* chunk, uint16_t& mesh_len, triangle* mesh, vec3f neighbour, triangle f1, triangle f2, vec3f pos)
  {
    //triangle t;
    //mesh[0] = t;
    
    if(chunk->get_block(neighbour) == 0)
    {
      for(byte i = 0; i < 3; i++)
      {
        f1.p[i] = f1.p[i] + pos;
        f2.p[i] = f2.p[i] + pos;
      }
      
      mesh[mesh_len++] = f1;
      mesh[mesh_len++] = f2;
    }
  }
  
public:
  MeshEntity build_mesh(Chunk* chunk, triangle* mesh)
  {    
    uint16_t mesh_len = 0;
    
    byte block;
    vec3f p;
    
    for(byte x = 0; x < CHUNK_SIZE; x++)
      for(byte y = 0; y < CHUNK_SIZE; y++)
        for(byte z = 0; z < CHUNK_SIZE; z++)
        {
          p = vec3f(x, y, z);
          block = chunk->get_block(p);

          if(block == 0) continue;

          add_mesh_face(chunk, mesh_len, mesh, vec3f(x, y + 1, z), m_top1, m_top2, p);
          add_mesh_face(chunk, mesh_len, mesh, vec3f(x, y - 1, z), m_bottom1, m_bottom2, p);

          add_mesh_face(chunk, mesh_len, mesh, vec3f(x + 1, y, z), m_right1, m_right2, p);
          add_mesh_face(chunk, mesh_len, mesh, vec3f(x - 1, y, z), m_left1, m_left2, p);

          add_mesh_face(chunk, mesh_len, mesh, vec3f(x, y, z - 1), m_back1, m_back2, p);
          add_mesh_face(chunk, mesh_len, mesh, vec3f(x, y, z + 1), m_front1, m_front2, p);
        }
    
    
    MeshEntity m = MeshEntity(mesh, mesh_len, mat4f::identity());
    return m;
  }



  void init_data()
  {
    m_front1 = triangle(1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f);
    m_front2 = triangle(1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f);

    m_back1 = triangle(0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f);
    m_back2 = triangle(0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f);

    m_left1 = triangle(0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f);
    m_left2 = triangle(0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f);

    m_right1 = triangle(1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f);
    m_right2 = triangle(1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f);

    m_top1 = triangle(0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f);
    m_top2 = triangle(0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f);

    m_bottom1 = triangle(1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f);
    m_bottom2 = triangle(1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f);
  }

};
