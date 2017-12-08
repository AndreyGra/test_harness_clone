#include <iostream> // input/output stream
#include <cstring>
#include <ctime>
#include <chrono>
#include <dirent.h> // directory header
#include <fstream>
#include "TimeKeeper.h"

//Useful Namespaces
using namespace std::chrono;

struct blobSettings
{

    int lowH,lowS,lowV;
    int highH,highS,highV;
    int minArea,maxArea;

};

struct BlobData
{
    int x;
    int y;
    float sizeConfidence;
    int size;
};

//Custom way to check for format
bool is_jpeg(const std::string& _file_name);

//Converts text line to integer
int LineToInt(std::ifstream& file);

//Interface to operate on blobsSettings
void                initializeBlobSettings  (blobSettings& settings, std::string& directoryName);
void                outputSettings          (blobSettings& settings);

//Interface to operate on data records
std::ofstream       initializeRecord        (std::string &directory);
void                writeToRecord           (TimeKeeper &OpenCV, TimeKeeper &Custom, std::ofstream &outputFileStream);
void                closeRecord             (std::ofstream &outputFileStream);

//Contains all logic relevant to image processing
//By design, you don't need openCV to compile the whole program without this function
void                processImage            (blobSettings &settings,
                                             TimeKeeper &OpenCVKeeper,
                                             TimeKeeper &CustomKeeper,
                                             std::string &directory,
                                             std::string &file_name
                                            );






//Provide argv[1] to be the number of folders:

int main (int argc, char** argv)
{
    auto custom_TimeKeeper = TimeKeeper();
    auto openCV_timekeeper = TimeKeeper();

    std::string::size_type sz;
    //int number_of_folders = std::stoi(argv[1],&sz);

    //This loop will iterate across all folders
    int number_of_folders = 4;

    for (int i = 1; i < number_of_folders ; ++i) {

        //Assuming that this will be opened in a parent folder containing several different folders
        //Directory will contain the name of the folder

        std::string directory = std::to_string(i);
        std::string pdir_directory = std::string(".")+std::string("//") + directory;

        auto settings = blobSettings();

        DIR *pdir = nullptr;                                                                                     // remember, it's good practice to initialise a pointer to NULL
        pdir = opendir ( pdir_directory.c_str() );                                                                // "." will refer to the current directory
        struct dirent *pent = nullptr;

        initializeBlobSettings(settings, directory);
        std::ofstream record_file = initializeRecord(directory);

        // Put this in to the folder containing images

        if (pdir == nullptr)                                                              // if pdir wasn't initialised correctly
        {                                                                                 // print an error message and exit the program
            std::cout << "\nERROR! pdir could not be initialised correctly";
            exit (3);
        }                                                                                  // end if

        while (pent = readdir (pdir) )                                                     // while there is still something in the directory to list
        {
            if (pent == nullptr)                                                           // if pent has not been initialised correctly
            {                                                                              // print an error message, and exit the program
                //std::cout << "\nERROR! pent could not be initialised correctly";
                //exit(3);
            }

            std::string file_name = pent->d_name;

            //Part that processes the image
            if ( is_jpeg(file_name) ) {

                processImage(settings,
                             openCV_timekeeper,
                             custom_TimeKeeper,
                             directory,
                             file_name);

                writeToRecord(openCV_timekeeper, custom_TimeKeeper, record_file);
            }

        }

        closeRecord(record_file);

        closedir (pdir);                                                            // finally, let's close the directory
    }

    //Loop ends here
    std::cin.get();
    return 0; // everything went OK
}


//Check for JPEG by checking last 4 characters of the string
bool is_jpeg(const std::string& _file_name)
{

    if (_file_name.length() > 3) {

        std::string _file_format = _file_name.substr((_file_name.length() - 5),(_file_name.length()));

        if ((_file_format) == ".jpg" ||(_file_format) == ".jpg\n" ) {
            return true;
        }
    }

    return false;
}

//convert blobSettings.txt to a C++ object format
void initializeBlobSettings(blobSettings& settings, std::string& directoryName)
{

    std::ifstream blobSettingsFile(directoryName + "//" + "blobsettings.txt");
    settings.lowH =     LineToInt(blobSettingsFile);
    settings.lowS =     LineToInt(blobSettingsFile);
    settings.lowV =     LineToInt(blobSettingsFile);
    settings.highH =    LineToInt(blobSettingsFile);
    settings.highS =    LineToInt(blobSettingsFile);
    settings.highV =    LineToInt(blobSettingsFile);
    settings.minArea =  LineToInt(blobSettingsFile);
    settings.maxArea =  LineToInt(blobSettingsFile);
    blobSettingsFile.close();
}


int LineToInt(std::ifstream& file)
{
    std::string line;
    getline(file, line);
    return std::stoi(line);
}

void outputSettings(blobSettings& settings) {

   std::cout << settings.lowH << std::endl;
   std::cout << settings.lowS << std::endl;
   std::cout << settings.lowV << std::endl;
   std::cout << settings.highH << std::endl;
   std::cout << settings.highS << std::endl;
   std::cout << settings.highV << std::endl;
   std::cout << settings.minArea << std::endl;
   std::cout << settings.maxArea << std::endl;
}

/*
 void ColourThreshold(Mat& originalImage,blobSettings& settings)
 {
    int lowH = settings.lowH;
    int lowS = settings.lowS;
    int lowV = settings.lowV;

    int highH = settings.highH;
    int highH = settings.highS;
    int highH = settings.highV;

	Mat imgHSV;
	cvtColor(originalImage, imgHSV, COLOR_BGR2HSV);	//Convert to HSV spectrum. Better than RGB for comparing colour similarity
	inRange(imgHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), originalImage); // Threshold the HSV image
	dilate(originalImage, originalImage, getStructuringElement(MORPH_ELLIPSE, Size(7, 7))); //morphological closing (fill small holes in the foreground)
	erode(originalImage, originalImage, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));
}
 */

/*
 * cv::SimpleBlobDetector::Params CreateParams(blobSettings& settings)
 * {
 *  int maximumArea = settings.maxArea;
 *  int minimumArea = settings.minArea;
 *
 *  cv::SimpleBlobDetector::Params params;
 *
	params.filterByColor = false;
	params.filterByConvexity = false;
	params.filterByInertia = false;
	params.filterByCircularity = false;

	params.filterByArea = true;
	params.minArea = settings.minArea;
	params.maxArea = settings.maxArea;

    return params;
 * }
 */

/*
 * void openCVMethod(cv::SimpleBlobDetector::Params &params, TimeKeeper& openCVKeeper)
 * {
 *  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
 *  std::vector<KeyPoint> keypoints;
 *
 *  openCVKeeper.start();
	detector -> detect(im, keypoints);
    openCVKeepr.endTimer();
 * }
 */

/*
 * void BlobPixel(Mat& binary, blobSettings& settings, TimeKeeper& customKeeper)
{
	vector<BlobData> blobsInImage;
	int minAreaModified = settings.minArea;
	int maxAreaModified = settings.maxArea;

    customKeeper.start();

	int numBlobsInImage = 2; // starts at 2 because 0 and 1 are used already
	for (int y = 0; y < binary.rows; y++) {
		uint8_t *row = (uint8_t*)binary.ptr(y);	//Get pointer to a row of pixels
		for (int x = 0; x < binary.cols && numBlobsInImage < 255; x++) {
			if (row[x] != 255) //255: fresh blob. 244 to 1: blob we've already examined. 0: negative space which isn't a blob
			{
				continue;
			}
			Rect rect;
			floodFill(binary, Point(x, y), numBlobsInImage, &rect, 0, 0, 8);	//Initially sets the blob to 2/255 brightness, then gradually increases it. This helps us separate blobs from each other.
			int blobSize = 0;

			for (int i = rect.y; i < (rect.y + rect.height); i++) {	//Rest of the entries will be specific pixel positions
				uint8_t *row2 = (uint8_t*)binary.ptr(i);
				for (int j = rect.x; j < (rect.x + rect.width); j++) {
					if (row2[j] == numBlobsInImage)
					{
						blobSize++;
					}
				}
			}

			if (blobSize > minAreaModified && blobSize < maxAreaModified)	//Check if size of blob is correct
			{
				float desired_area = ((float)maxAreaModified - (float)minAreaModified) / 2.0f;
				float A = -100 / (desired_area * desired_area);

				BlobData blob;
				blob.x = rect.x + rect.width / 2;
				blob.y = rect.y + rect.height / 2;
				blobsInImage.push_back(blob);
			}

			numBlobsInImage++;
		}
	}

    customKeeper.endTimer();
}
 */

void writeToRecord(TimeKeeper &OpenCV, TimeKeeper &Custom, std::ofstream &outputFileStream)
{
    outputFileStream << OpenCV.getTime();
    outputFileStream << " ";
    outputFileStream << Custom.getTime();
    outputFileStream << std::endl;

}

std::ofstream initializeRecord(std::string &directory)
{

    std::ofstream myfile;
    myfile.open(directory+"//"+"records.txt");//, std::ios::app | std::ios::trunc);

    return myfile;
}

void closeRecord(std::ofstream &outputFileStream)
{
    outputFileStream.close();
}

void processImage(blobSettings &settings,
                  TimeKeeper &OpenCVKeeper,
                  TimeKeeper &CustomKeeper,
                  std::string &directory,
                  std::string &file_name
                 )
{
    //Mat binary_image_openCV, binary_image_custom;
    //binary_image_openCV = imread(directory+file_name,...);
    //ColourThreshold(binary_image, settings)

    //binary_image_custom = binary_image_openCV;

    //blobPixel(binary_image_custom,settings,CustomKeeper)

    //cv::SimpleBlobDetector::Params parameters = CreateParams(settings);
    //openCVMethod(parameters,OpenCVKeeper);
}
