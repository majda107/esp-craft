#define CHUNK_SIZE 4

class Chunk
{
public:
  byte data[CHUNK_SIZE][CHUNK_SIZE];

  void fill(byte dat)
  {
    for(byte x = 0; x < CHUNK_SIZE; x++)
      for(byte z = 0; z < CHUNK_SIZE; z++)
        data[x][z] = dat;
  }

  byte get_block(vec3f p)
  {
    if(p.x < 0 || p.x >= CHUNK_SIZE) return 0;
    if(p.y < 0 || p.y >= 1) return 0;
    if(p.z < 0 || p.z >= CHUNK_SIZE) return 0;

    return data[(byte)p.x][(byte)p.z];
  }
};
