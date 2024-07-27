#define SAVE_INTERMEDIATE
#define OUPUT_DIR "sample images/output/"

#include<ctime>
#include<iostream>
#include "Image.h"
#include "pgmio.h"
#include "pbmio.h"
#include "segment.h"
#include "tableDetection.h"

using namespace std;

void extractFilename(string filename)
{
    int p1 = filename.find_last_of("/");
    if(p1==string::npos) 
    {    
        p1 = filename.find_last_of("\\");
        if(p1==string::npos) p1=0; else p1++;
    }
    else p1++;    

    int p2 = filename.find_last_of(".");
    if(p2==string::npos)
        p2 = filename.size();
    p2 = p2 - p1;
    image_filename = OUPUT_DIR + filename.substr(p1, p2);
}

int main(int argc, char* argv[])
{
    try
    {
        // get input filename from commandline
        string filename = argv[1];

        extractFilename(filename);

        clock_t beg = clock();
        Image image = read_pgm(filename); 

        
        Image resultImg(image.width, image.height, 255);
        vector<ImageSegment> segments = detectTable(image);
        
        for(int i=0; i<segments.size();i++)
            if(segments[i].isTable)
                boundingBox::drawBoundingBox(resultImg, segments[i].outer);        
        
        write_pbm(image_filename+"-tables.pbm", resultImg);

        clock_t end = clock();       
        double elapsed = double(end-beg)/CLOCKS_PER_SEC;
        cout << "Time elapsed: " << elapsed << "s \n";
    }
    catch(string s)
    {
        cout << s << "\n";
    }
    catch(const char* s)
    {
        cout << s << "\n";
    }


    return 0;
}