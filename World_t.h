#include"Header.h"

class ChunkThreadPool
{
public:

    ChunkThreadPool(int threads) : shutdown_(false)
    {
        threads_.reserve(threads);
        for (int i = 0; i < threads; ++i)
            threads_.emplace_back(std::bind(&ChunkThreadPool::threadEntry, this, i));
    }

    ~ChunkThreadPool()
    {
        {
            // Unblock any threads and tell them to stop
            std::unique_lock <std::mutex> l(lock_);

            shutdown_ = true;
            condVar_.notify_all();
        }

        // Wait for all threads to stop
        std::cout << "Joining threads" << std::endl;
        for (auto& thread : threads_)
            thread.join();
    }

    void doJob(std::function <void(void)> func)
    {
        // Place a job on the queu and unblock a thread
        std::unique_lock <std::mutex> l(lock_);

        jobs_.emplace(std::move(func));
        condVar_.notify_one();
    }

protected:

    void threadEntry(int i)
    {
        std::function <void(void)> job;

        while (1)
        {
            {
                std::unique_lock <std::mutex> l(lock_);

                while (!shutdown_ && jobs_.empty())
                    condVar_.wait(l);

                if (jobs_.empty())
                {
                    // No jobs to do and we are shutting down
                    std::cerr << "Thread " << i << " terminates" << std::endl;
                    return;
                }

               // std::cerr << "Thread " << i << " does a job" << std::endl;
                job = std::move(jobs_.front());
                jobs_.pop();
            }

            // Do the job without holding any locks
            job(); 
        }

    }

    std::mutex lock_;
    std::condition_variable condVar_;
    bool shutdown_;
    std::queue <std::function <void(void)>> jobs_;
    std::vector <std::thread> threads_;
};

class Chunk {

private:
public:
	static const int VERTEX_COUNT = 16;
	static const int SIZE = 250;
	Material* material;
	Mesh* mesh;

	int x,z;

	glm::mat4 matrix_transformation;

	Chunk(int x,int z,
		Material* material,
		Mesh * mesh) {

		this->x = x;
		this->z=z;
		this->material = material;
		this->mesh = mesh;

		this->matrix_transformation = glm::mat4(1.f);

	}



};
typedef struct chunkdata {
    bool finished, started;
    Vertex* vertices;
    GLuint* indices;
    int vertex_count, index_count;

} ChunkData;

glm::vec3 normOfPlane(int x, int z, FastNoiseLite * generator, float amplitude) {

    float p1 = generator->GetNoise((float)x-1 , (float)z ) * amplitude  ;
    float p2 = generator->GetNoise((float)x+1 , (float)z ) * amplitude  ;
    float p3 = generator->GetNoise((float)x , (float)z-1 ) * amplitude  ;
    float p4 = generator->GetNoise((float)x , (float)z+1 ) * amplitude  ;

    return glm::normalize(glm::vec3(p1 - p2, 2.f, p3 - p4));
}
void load(ChunkData* cd, int i, int j, int count, FastNoiseLite * generator)
{

    float amplitude = 5000;
    for (int z = 0; z < count; z++) {
        for (int x = 0; x < count; x++) {
            float tx = (float)x / (count - 1) * Chunk::SIZE + i * Chunk::SIZE;

            float ty = generator->GetNoise((float)x + (float)i * (count - 1), (float)z + (float)j * (count - 1)) * amplitude;//  gen.GetValue((float)x + (float)i * (count - 1), (float)z + (float)j * (count - 1), 1)*56;

            float tz = (float)z / (count - 1) * Chunk::SIZE + j * Chunk::SIZE;

            cd->vertices[z * count + x].position = glm::vec3(tx, ty, tz);
            cd->vertices[z * count + x].color = glm::vec3(1.f, 1.f, 1.f);
            cd->vertices[z * count + x].texcoord = glm::vec2((float)x / (count - 1), (float)z / (count - 1));
            cd->vertices[z * count + x].normal = normOfPlane(x + i * (count - 1), z + j * (count - 1), generator, amplitude);
        }


    }

    int pointer = 0;
    for (int z = 0; z < count - 1; z++) {
        for (int x = 0; x < count - 1; x++) {

            GLuint topLeft = (x * count) + z;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = ((x + 1) * count) + z;
            GLuint bottomRight = bottomLeft + 1;

            cd->indices[pointer++] = topLeft;
            cd->indices[pointer++] = bottomRight;
            cd->indices[pointer++] = topRight;
            cd->indices[pointer++] = topLeft;
            cd->indices[pointer++] = bottomLeft;
            cd->indices[pointer++] = bottomRight;


        }
    }
    cd->finished = true;
}


class World {
private:

	long seed;

	Entity* entities;
	Chunk** chunks;
    Chunk** loaded_chunks;

    ChunkThreadPool chunkPool;
	ChunkData* loading_chunks;
    std::vector<std::function<void()>> jobs; 

    FastNoiseLite generator;

	GLLoader* loader;

public:
    static const int LOADING_LIMIT = 1024;
    static const int RENDER_DISTANCE = 3;
    static const int MAX_THREADS = 5;
    static const int ENTITY_LIMIT = 256;

	World(GLLoader* loader) :chunkPool(World::MAX_THREADS) ,generator() {
		generator.SetSeed(seed);
		generator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
		generator.SetFractalType(FastNoiseLite::FractalType_FBm);
        generator.SetFrequency(.005);

		generator.SetFractalOctaves(5);



		chunks = (Chunk**)malloc(sizeof(Chunk*) *(2* RENDER_DISTANCE+1) * (2 * RENDER_DISTANCE + 1));//new Chunk[RENDER_DISTANCE*RENDER_DISTANCE];
        for (int i = 0; i < (2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1); i++) {
            chunks[i] = NULL;
        }
		loaded_chunks = (Chunk**)malloc(sizeof(Chunk*) *(LOADING_LIMIT) * ( LOADING_LIMIT ));//new Chunk[RENDER_DISTANCE*RENDER_DISTANCE];
        for (int i = 0; i < ( LOADING_LIMIT ) * ( LOADING_LIMIT ); i++) {
            loaded_chunks[i] = NULL;
        }
		//entities = new Entity[ENTITY_LIMIT];
		loading_chunks = new ChunkData[(2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1)];
        for (int i = 0; i < (2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1); i++) {

            loading_chunks[i].finished = false;
            loading_chunks[i].started = false;
            loading_chunks[i].vertex_count = Chunk::VERTEX_COUNT * Chunk::VERTEX_COUNT;
            loading_chunks[i].index_count = (Chunk::VERTEX_COUNT - 1) * (Chunk::VERTEX_COUNT - 1) * 6;
            loading_chunks[i].vertices = new Vertex[loading_chunks[i].vertex_count];
            loading_chunks[i].indices = new GLuint[loading_chunks[i].index_count];

        }


		this->loader = loader;
	}
	~World() {
        for (int i = 0; i < (2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1); i++) {
		    delete loading_chunks[i].vertices;
		    delete loading_chunks[i].indices;
        }
		delete chunks;
		delete loading_chunks;
	}

	void loadChunks(glm::vec3 position, Material * material) {
		for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++) {
			int chunk_x = floorf(position.x / Chunk::SIZE) + dx;
            for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++) {
				int chunk_z = floorf(position.z / Chunk::SIZE) + dz;

				int i = ((dx + RENDER_DISTANCE)* (2 * RENDER_DISTANCE+1)) + (dz + RENDER_DISTANCE );
                int li =(chunk_x)*LOADING_LIMIT + chunk_z;
                

                if(loaded_chunks[li] == NULL){
                     
                    if (loading_chunks[i].finished) {
                        Mesh* m = loader->loadToMesh(loading_chunks[i].vertices, loading_chunks[i].indices, loading_chunks[i].vertex_count, loading_chunks[i].index_count);

                        
                        Chunk * chunk = new Chunk(chunk_x, chunk_z, material, m);
                       
                        loaded_chunks[li] = chunk;
                    chunks[i] = loaded_chunks[li];
                        loading_chunks[i].finished = false;
                        loading_chunks[i].started = false;
                        continue;
                    }
                    if (!loading_chunks[i].started) {

                        loading_chunks[i].finished = false;
                        loading_chunks[i].started = true;

                        chunkPool.doJob(std::bind(load, &loading_chunks[i], chunk_x, chunk_z, 16, &generator));
                        continue;
                    }
                }else{
                    chunks[i] = loaded_chunks[li];
                }


			}
		}


	}

	Chunk** getChunks() {


		return chunks;
	}


};