//
//
#include <iostream>
#include <FreeImage.h>
#include <fstream>
#include <omp.h>
#include <sys/time.h>
#include <iomanip>

#include "Object.h"
#include "SceneParser.h"
#include "Camera.h"
#include "RenderInfo.h"
#include "Image.h"
#include "RayTracer.h"

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>

using namespace std;

namespace fs = ::boost::filesystem;

// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
static void get_all_scenes(const fs::path& root, const string& ext, vector<fs::path>& ret)
{
    if(!fs::exists(root) || !fs::is_directory(root)) return;

    fs::directory_iterator it(root);
    fs::directory_iterator endit;
    //fs::recursive_directory_iterator it(root);
    //fs::recursive_directory_iterator endit;

    while(it != endit)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ext)
        	ret.push_back(it->path());
        ++it;
    }

}

static void render_scene(string &fileName)
{

	RenderInfo *renderInfo = SceneParser::readFile(fileName.c_str());

	RayTracer rayTracer;
	Image *img = rayTracer.rayTrace(fileName, renderInfo->camera, renderInfo->scene, renderInfo->width, renderInfo->height, renderInfo->maxDepth);

	fs::path p = fileName;

	string output;
	if (renderInfo->outputFile.empty()) {
		// No output file was given
		output = fileName + "_result.png";
	}
	else {
		output = p.parent_path().generic_string() + "/" + renderInfo->outputFile;
	}

	img->saveImage(output);

	delete img;
	delete renderInfo;
}


#include "Model.h"

int main()
{
//	string path = "./scenes/b/cube.obj";
//	Model m = Model(path);
//
//	return 0;


	struct timeval start, end;
	GLfloat time, totalTime;



	FreeImage_Initialise();

	fs::path directory = "./scenes/b";
	string ext = ".test";
	vector<fs::path> files;
	get_all_scenes(directory, ext, files);

	Image *img = nullptr;
	string fileName;

	cout << "Ray Tracer working..." << endl;

	int tid;

	for (fs::path p : files) {

		gettimeofday(&start, NULL);


		std::cout << "\tRendering " << p.filename() << "..." << endl;
		string file = p.generic_string();
		render_scene(file);

		std::cout << "\tFinished rendering " << p.filename() << "..." << endl;

		gettimeofday(&end, NULL);

		time = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
		cout << "\tTime took: " << time << setprecision(1) << " seconds. " << endl;
		totalTime += time;

	}


	cout << "Finished working. Total Time: " << totalTime / 60 << setprecision(2) << " minutes." << endl;

	FreeImage_DeInitialise();
	return 0;
}



//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

//int main()
//{
//	vec3 o1 = vec3(0.0, 0.0, 0.0);
//	vec3 d1 = vec3(1.0, -1.0, 1.0);
//	Ray r1(o1, d1);
//
//	glm::mat4 translate = glm::translate(glm::mat4(1.0f), vec3(5.0, 0.0, 0.0));
//
//	Ray r2 = translate * r1;
//	r1.print();
//	r2.print();
//
//}

//
//#define WIDTH 200
//#define HEIGHT 300
//#define BPP 24
//
//using namespace std;
//
//int main()
//{
//	FreeImage_Initialise();
//	FIBITMAP *bitmap = FreeImage_Allocate(WIDTH, HEIGHT, BPP);
//	RGBQUAD color;
//
//	for (int i = 0 ; i < WIDTH ; ++i) {
//		for (int j = 0 ; j < HEIGHT ; ++j) {
//
//			color.rgbGreen = 255.0f;
//			FreeImage_SetPixelColor(bitmap, i, j, &color);
//		}
//	}
//
//	FreeImage_Save(FIF_PNG, bitmap, "test.png", 0);
//
//	FreeImage_DeInitialise();
//
//}
