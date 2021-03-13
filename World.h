#pragma once
#include "Header.h"
enum Biome
{
    TROPICAL_RAINFOREST,
    TEMPERATE_RAINFOREST,
    TROPICAL,
    TEMPERATE,
    SAVANA,
    TAIGA,
    GRASSLAND,
    DESERT,
    TUNDRA,
    OCEAN
};

class Chunk
{

private:
public:
    static const int VERTEX_COUNT = 16;
    static const int BIOME_DETAIL = 8;
    static const int SIZE = 16 * 10;
    Mesh *mesh;
    Material *material;

  

    std::vector<Entity *> tileEntities;

    int x, z;

    glm::mat4 matrix_transformation;

    Chunk(int x, int z,
          Mesh *mesh,
          Material *material)
    {
        this->x = x;
        this->z = z;
        this->material = material;
        this->mesh = mesh;

        this->matrix_transformation = glm::mat4(1.f);
    }
};
class World
{
private:
    long seed;

    Chunk **chunks;
    Chunk **loaded_chunks;

    std::vector<Entity *> *entities;


    FastNoiseLite generator;

    GLLoader *loader;


    Entity * skyBox;
    Model * model_rock_1;
    Model * model_tree_1;
    Model * model_plant,*model_leaves_1;

public:
    static const int LOADING_LIMIT = 512;
    static const int RENDER_DISTANCE = 16;
    static const int ENTITY_LIMIT = 256;

    World(GLLoader *loader) : generator()
    {
        generator.SetSeed(seed);
        entities = new std::vector<Entity *>;
        chunks = (Chunk **)malloc(sizeof(Chunk *) * (2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1)); //new Chunk[RENDER_DISTANCE*RENDER_DISTANCE];
        for (int i = 0; i < (2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1); i++)
        {
            chunks[i] = NULL;
        }

        this->loader = loader;

        Vertex vertices[] =
            {
                //Position								//Color							//Texcoords					//Normals
                glm::vec3(-0.5f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 1.f, 0.f),
                glm::vec3(-0.5f, 0.0f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
                glm::vec3(0.5f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
                glm::vec3(0.5f, 1.f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 1.f, 0.f),

                glm::vec3(0.0f, 1.f, -0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 1.f, 0.f),
                glm::vec3(0.0f, 0.f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
                glm::vec3(0.0f, 0.f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
                glm::vec3(0.0f, 1.0f, 0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 1.f, 0.f)};

        GLuint indices[] =
            {
                0, 1, 2, //Triangle 1
                0, 2, 3, //Triangle 2
                4, 5, 6, //Triangle 1
                4, 6, 7  //Triangle 2
            };

        Vertex v_cube[] =
        {
            //Position                              //Color                         //Texcoords                 //Normals
            glm::vec3(1.0f-.5f, 0.0f-.15f, 1.0f-.5f),           glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(1.f, 1.f),        glm::vec3(0.f, -1.f, 0.f),
            glm::vec3(1.0f-.5f, 0.0f-.15f, 0.0f-.5f),          glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(1.f, 0.f),        glm::vec3(0.f, -1.f, 0.f),
            glm::vec3(0.0f-.5f, 0.0f-.15f, 1.0f-.5f),           glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 1.f),        glm::vec3(0.f, -1.f, 0.f),
            glm::vec3(0.0f-.5f, 0.0f-.15f, 0.0f-.5f),            glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 0.f),        glm::vec3(0.f, -1.f, 0.f),

            glm::vec3(1.0f-.5f, 1.0f-.15f, 1.0f-.5f),           glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 1.f),        glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.0f-.5f, 1.0f-.15f, 0.0f-.5f),          glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 0.f),        glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.0f-.5f, 1.0f-.15f, 1.0f-.5f),           glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(0.f, 1.f),        glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(0.0f-.5f, 1.0f-.15f, 0.0f-.5f),            glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(0.f, 0.f),        glm::vec3(0.f, 1.f, 0.f),

            glm::vec3(0.0f-.5f, 1.0f-.15f, 1.0f-.5f),           glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 1.f),        glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.0f-.5f, 1.0f-.15f, 0.0f-.5f),          glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 0.f),        glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.0f-.5f, 0.0f-.15f, 1.0f-.5f),           glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 1.f),        glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.0f-.5f, 0.0f-.15f, 0.0f-.5f),            glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 0.f),        glm::vec3(0.f, 0.f, 1.f),

            glm::vec3(1.0f-.5f, 1.0f-.15f, 1.0f-.5f),           glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 1.f),        glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(1.0f-.5f, 1.0f-.15f, 0.0f-.5f),          glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 0.f),        glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(1.0f-.5f, 0.0f-.15f, 1.0f-.5f),           glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 1.f),        glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(1.0f-.5f, 0.0f-.15f, 0.0f-.5f),            glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 0.f),        glm::vec3(0.f, 0.f, -1.f),

            glm::vec3(1.0f-.5f, 1.0f-.15f, 0.0f-.5f),           glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 1.f),        glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(1.0f-.5f, 0.0f-.15f, 0.0f-.5f),           glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(1.f, 0.f),        glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.0f-.5f, 1.0f-.15f, 0.0f-.5f),          glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(0.f, 1.f),        glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.0f-.5f, 0.0f-.15f, 0.0f-.5f),            glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 0.f),        glm::vec3(1.f, 0.f, 0.f),

            glm::vec3(1.0f-.5f, 1.0f-.15f, 1.0f-.5f),           glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(1.f, 1.f),        glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(1.0f-.5f, 0.0f-.15f, 1.0f-.5f),           glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(1.f, 0.f),        glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(0.0f-.5f, 1.0f-.15f, 1.0f-.5f),          glm::vec3(186.f/255, 232.f/255, 255.f/255),       glm::vec2(0.f, 1.f),        glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(0.0f-.5f, 0.0f-.15f, 1.0f-.5f),            glm::vec3(21.f/255, 53.f/255, 69.f/255),       glm::vec2(0.f, 0.f),        glm::vec3(-1.f, 0.f, 0.f)
        };

        GLuint i_cube[] =
        {
            0, 1, 2,
            1, 2, 3,

            0+4, 1+4, 2+4,
            1+4, 2+4, 3+4,

            0+8, 1+8, 2+8,
            1+8, 2+8, 3+8,

            0+12, 1+12, 2+12,
            1+12, 2+12, 3+12,

            0+16, 1+16, 2+16,
            1+16, 2+16, 3+16,

            0+20, 1+20, 2+20,
            1+20, 2+20, 3+20
        };

        Mesh * cube  = loader->loadToMesh(v_cube, i_cube, sizeof(v_cube) / sizeof(Vertex), sizeof(i_cube) / sizeof(GLuint));
        skyBox = new Entity(glm::vec3(0), glm::vec3(0,0,0), glm::vec3(0),glm::vec3(Chunk::SIZE*(World::RENDER_DISTANCE*2+1)*2),  new Model(cube, new TexturedMaterial(1, 0, 0, 0, 0))); 
        entities->push_back(skyBox);

        Mesh * plant_1 = loader->loadToMesh( vertices, indices, sizeof(vertices) / sizeof(Vertex), sizeof(indices) / sizeof(GLuint));
        model_plant = new Model(plant_1, new TexturedMaterial(0.3, .8, 0, Texture::TALL_GRASS, 0));

        Mesh * rock_1 = loader->loadToMesh( "res/obj/RockSet.obj" );
        model_rock_1 = new Model(rock_1, new TexturedMaterial(0.2, .7, 0, Texture::SMOOTH_STONE, 0));

        Mesh * tree_1 = loader->loadToMesh( "res/obj/MapleTreeStem.obj" );
        model_tree_1 = new Model(tree_1, new TexturedMaterial(0.2, .7, 0, Texture::BARK, 0));

        Mesh * leaves_1 = loader->loadToMesh( "res/obj/MapleTreeLeaves.obj" );
        model_leaves_1 = new Model(leaves_1, new TexturedMaterial(0.2, .7, 0, Texture::LEAVES, 0));

    

    }
    ~World()
    {
        delete model_rock_1;
        delete chunks;
    }

    glm::vec3 normOfPlane(float x, float z, float amplitude =1)
    {
        float p1 = getHeight(x - 1, z, amplitude);
        float p2 = getHeight(x + 1, z, amplitude);
        float p3 = getHeight(x, z - 1, amplitude);
        float p4 = getHeight(x , z+ 1, amplitude);

        return glm::normalize(glm::vec3(p1 - p2, 3.f, p3 - p4));
    }

    float getHeight(float x, float z, float maxHeight =1 )
    {

        float ty = generator.GetNoise(x, z) * maxHeight;
        
        return ty;
    }

    Chunk *loadChunk(int chunk_x, int chunk_z)
    {


        Vertex *vertices = new Vertex[Chunk::VERTEX_COUNT * Chunk::VERTEX_COUNT];
        GLuint *indices = new GLuint[(Chunk::VERTEX_COUNT - 1) * 6 * (Chunk::VERTEX_COUNT - 1)];

        //GENERAL BIOME SETTINGS
        generator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
        generator.SetFractalType(FastNoiseLite::FractalType_FBm);
        generator.SetFrequency(.002);
        generator.SetFractalGain(.8);
        generator.SetFractalOctaves(7);
        generator.SetFractalWeightedStrength(.8);


        float amplitude = 500;
        for (int z = 0; z < Chunk::VERTEX_COUNT; z++)
        {
            for (int x = 0; x < Chunk::VERTEX_COUNT; x++)
            {

                int i = z * Chunk::VERTEX_COUNT + x;

                float tx = (float)x / (Chunk::VERTEX_COUNT - 1) * Chunk::SIZE + chunk_x * Chunk::SIZE;
                float res = getHeight(x + chunk_x * (Chunk::VERTEX_COUNT - 1), z + chunk_z * (Chunk::VERTEX_COUNT - 1), amplitude);
                float tz = (float)z / (Chunk::VERTEX_COUNT - 1) * Chunk::SIZE + chunk_z * Chunk::SIZE;
                vertices[i].position = glm::vec3(tx, res, tz);
                vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
                vertices[i].texcoord = glm::vec2((float)x / (Chunk::VERTEX_COUNT - 1), (float)z / (Chunk::VERTEX_COUNT - 1));
                vertices[i].normal = normOfPlane(x + chunk_x * (Chunk::VERTEX_COUNT - 1), z + chunk_z * (Chunk::VERTEX_COUNT - 1), amplitude);
            }
        }

        int pointer = 0;
        for (int z = 0; z < Chunk::VERTEX_COUNT - 1; z++)
        {
            for (int x = 0; x < Chunk::VERTEX_COUNT - 1; x++)
            {

                GLuint topLeft = (x * Chunk::VERTEX_COUNT) + z;
                GLuint topRight = topLeft + 1;
                GLuint bottomLeft = ((x + 1) * Chunk::VERTEX_COUNT) + z;
                GLuint bottomRight = bottomLeft + 1;

                indices[pointer++] = topLeft;
                indices[pointer++] = bottomRight;
                indices[pointer++] = topRight;
                indices[pointer++] = topLeft;
                indices[pointer++] = bottomLeft;
                indices[pointer++] = bottomRight;
            }
        }

        Mesh *mesh = loader->loadToMesh(vertices, indices, Chunk::VERTEX_COUNT * Chunk::VERTEX_COUNT, (Chunk::VERTEX_COUNT - 1) * (Chunk::VERTEX_COUNT - 1) * 6);
        Chunk *chunk = new Chunk(chunk_x, chunk_z, mesh, new Material(.2, .8, .0));
        
            for (int i = 0; i < 16; i++)
            {
                int j = rand() % (Chunk::VERTEX_COUNT * Chunk::VERTEX_COUNT);
                float rx = vertices[j].position.x + ((float)(rand() % 10-5)/10*50);
                float rz = vertices[j].position.z+ ((float)(rand() % 10-5)/10*50);
                float ry = vertices[j].position.y;
                if (vertices[j].position.y > 1 && vertices[j].position.y <= 100 && glm::dot(vertices[j].normal, glm::vec3(0, 1, 0)) > .6 )
                {
                    int d = rand() % 20;
                    if(d < 1){

                        int s = 1+rand() % 5;
                        entities->push_back(new Entity(glm::vec3(rx, ry-.15, rz), glm::vec3(0), glm::vec3(0), glm::vec3(s), model_tree_1 ));
                    
                        entities->push_back(new Entity(glm::vec3(rx, ry-.15, rz), glm::vec3(0), glm::vec3(0), glm::vec3(s ), model_leaves_1 , true));
                    } else if(d < 5){
                        entities->push_back(new Entity(glm::vec3(rx, ry-.15, rz), glm::vec3(0), glm::vec3(0), glm::vec3(2+rand() % 5 ), model_rock_1 ));
                    }else{
                        entities->push_back(new Entity(glm::vec3(rx, ry-.5, rz), glm::vec3(0), glm::vec3(0), glm::vec3(15+rand() % 5 ), model_plant ));
                    }
                   

                }
            }
        

        return chunk;
    }

    void loadChunks(glm::vec3 position)
    {
        for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        {
            int chunk_x = floorf(position.x / Chunk::SIZE) + dx;
            for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
            {
                int chunk_z = floorf(position.z / Chunk::SIZE) + dz;

                int i = ((dx + RENDER_DISTANCE) * (2 * RENDER_DISTANCE + 1)) + (dz + RENDER_DISTANCE);

                int li = (chunk_x + (LOADING_LIMIT / 2)) * LOADING_LIMIT + chunk_z + (LOADING_LIMIT / 2);
                if (li > LOADING_LIMIT * LOADING_LIMIT || li < 0)
                {
                    continue;
                }

                if ( chunks[i] == NULL)
                {
                     chunks[i] = loadChunk(chunk_x, chunk_z);
                }
            }
        }
    }
    void reLoadChunks(glm::vec3 position)
    {
        entities->clear();
        entities->push_back(skyBox);

        for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        {
            int chunk_x = floorf(position.x / Chunk::SIZE) + dx;
            for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
            {
                int chunk_z = floorf(position.z / Chunk::SIZE) + dz;

                int i = ((dx + RENDER_DISTANCE) * (2 * RENDER_DISTANCE + 1)) + (dz + RENDER_DISTANCE);

                int li = (chunk_x + (LOADING_LIMIT / 2)) * LOADING_LIMIT + chunk_z + (LOADING_LIMIT / 2);
                if (li > LOADING_LIMIT * LOADING_LIMIT || li < 0)
                {
                    continue;
                }
                if ( chunks[i] != NULL ){
                    delete chunks[i];
                    chunks[i] = NULL;
                }

                if ( chunks[i] == NULL)
                {
                     chunks[i] = loadChunk(chunk_x, chunk_z);
                }
            }
        }
    }
    void updateSkyBox(glm::vec3 position){

        skyBox->setPosition(position);
    }
    std::vector<Entity *> *getEntities()
    {
        return entities;
    }
    Chunk **getChunks()
    {

        return chunks;
    }
};