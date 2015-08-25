
#include "opencv2/objdetect.hpp"
#include "haarconsts.hpp"
#include <fstream>

#include <iostream>

#define ICV_HAAR_SIZE_NAME            "size"
#define ICV_HAAR_STAGES_NAME          "stages"
#define ICV_HAAR_TREES_NAME           "trees"
#define ICV_HAAR_FEATURE_NAME         "feature"
#define ICV_HAAR_RECTS_NAME           "rects"
#define ICV_HAAR_TILTED_NAME          "tilted"
#define ICV_HAAR_THRESHOLD_NAME       "threshold"
#define ICV_HAAR_LEFT_NODE_NAME       "left_node"
#define ICV_HAAR_LEFT_VAL_NAME        "left_val"
#define ICV_HAAR_RIGHT_NODE_NAME      "right_node"
#define ICV_HAAR_RIGHT_VAL_NAME       "right_val"
#define ICV_HAAR_STAGE_THRESHOLD_NAME "stage_threshold"
#define ICV_HAAR_PARENT_NAME          "parent"
#define ICV_HAAR_NEXT_NAME            "next"



using namespace std;
using namespace cv;

static CvHaarClassifierCascade*
myLoadHaarClassifierCascade();

static CvHaarClassifierCascade*
myCreateHaarClassifierCascade( int stage_count );

static void*
writeHaarClassifierconsts( const void* struct_ptr , char* filename );


/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt2.xml";
CascadeClassifier face_cascade;


/** @function main */
int main( void )
{


    CvHaarClassifierCascade* oldCascade = (CvHaarClassifierCascade*)cvLoad(face_cascade_name.c_str(), 0);



    CvHaarClassifierCascade* oldCascade2 = myLoadHaarClassifierCascade();

    writeHaarClassifierconsts(oldCascade,"haarclassifierconsts.hpp");
    writeHaarClassifierconsts(oldCascade2,"haarclassifierconsts2.hpp");
    return 0;
}


static void*
writeHaarClassifierconsts( const void* struct_ptr,char* filename )
{
    vector<float> stage_classifier_threshold;
    vector<int> stage_classifier_count;
    vector<float> classifier_threshold;
    vector<int> classifier_left;
    vector<int> classifier_right;
    vector<float> classifier_alpha;
    vector<float> classifier_alpha2;

    vector<int> haar_feature_rect;
    vector<float> haar_feature_weight;

    CvHaarClassifierCascade* cascade = NULL;

    int i, j, k, l ,n;
    const CvHaarClassifierCascade* cascade_src =
        (const CvHaarClassifierCascade*) struct_ptr;

    n = cascade_src->count;
    cascade = myCreateHaarClassifierCascade(n);
    cascade->orig_window_size = cascade_src->orig_window_size;

    for( i = 0; i < n; ++i )
    {
        cascade->stage_classifier[i].parent = cascade_src->stage_classifier[i].parent;

        cascade->stage_classifier[i].next = cascade_src->stage_classifier[i].next;

        cascade->stage_classifier[i].child = cascade_src->stage_classifier[i].child;

        cascade->stage_classifier[i].threshold = cascade_src->stage_classifier[i].threshold;
        stage_classifier_threshold.push_back(cascade_src->stage_classifier[i].threshold);


        cascade->stage_classifier[i].count = 0;
        cascade->stage_classifier[i].classifier =
            (CvHaarClassifier*) cvAlloc( cascade_src->stage_classifier[i].count
                                         * sizeof( cascade->stage_classifier[i].classifier[0] ) );

        cascade->stage_classifier[i].count = cascade_src->stage_classifier[i].count;
        stage_classifier_count.push_back(cascade_src->stage_classifier[i].count);
        for( j = 0; j < cascade->stage_classifier[i].count; ++j )
            cascade->stage_classifier[i].classifier[j].haar_feature = NULL;

        for( j = 0; j < cascade->stage_classifier[i].count; ++j )
        {
            const CvHaarClassifier* classifier_src =
                &cascade_src->stage_classifier[i].classifier[j];
            CvHaarClassifier* classifier =
                &cascade->stage_classifier[i].classifier[j];

            classifier->count = classifier_src->count;

            classifier->haar_feature = (CvHaarFeature*) cvAlloc(
                                           classifier->count * ( sizeof( *classifier->haar_feature ) +
                                                   sizeof( *classifier->threshold ) +
                                                   sizeof( *classifier->left ) +
                                                   sizeof( *classifier->right ) ) +
                                           (classifier->count + 1) * sizeof( *classifier->alpha ) );
            classifier->threshold = (float*) (classifier->haar_feature+classifier->count);

            classifier->left = (int*) (classifier->threshold + classifier->count);
            classifier->right = (int*) (classifier->left + classifier->count);
            classifier->alpha = (float*) (classifier->right + classifier->count);

            for( k = 0; k < classifier->count; ++k )
            {

                CvHaarFeature* feature = &classifier_src->haar_feature[k];

                for( l = 0; l < CV_HAAR_FEATURE_MAX ; ++l )
                {
                    haar_feature_rect.push_back(feature->rect[l].r.x);
                    haar_feature_rect.push_back(feature->rect[l].r.y);
                    haar_feature_rect.push_back(feature->rect[l].r.width);
                    haar_feature_rect.push_back(feature->rect[l].r.height);

                    haar_feature_weight.push_back(feature->rect[l].weight);

                }

                classifier->haar_feature[k] = classifier_src->haar_feature[k];


                classifier->threshold[k] = classifier_src->threshold[k];
                classifier_threshold.push_back(classifier_src->threshold[k]);
                classifier->left[k] = classifier_src->left[k];
                classifier_left.push_back(classifier_src->left[k]);
                classifier->right[k] = classifier_src->right[k];
                classifier_right.push_back(classifier_src->right[k]);
                classifier->alpha[k] = classifier_src->alpha[k];
                classifier_alpha.push_back(classifier_src->alpha[k]);
            }
            classifier->alpha[classifier->count] =
                classifier_src->alpha[classifier->count];
            classifier_alpha2.push_back(classifier_src->alpha[classifier->count]);
        }
    }

    FILE* fp = fopen(filename, "wt");
    char* fmt;


    fprintf(fp,"const int ClassifierCascadeCount=%d;\n",cascade_src->count);
    fprintf(fp,"const int cascadewidth=%d;\n",cascade_src->orig_window_size.width);
    fprintf(fp,"const int cascadeheight=%d;\n\n",cascade_src->orig_window_size.height);

    fprintf(fp,"const float stage_classifier_threshold[] = \n{\n");

    for( i = 0; i < stage_classifier_threshold.size(); ++i )
    {
        if (i == stage_classifier_threshold.size()-1)
            fmt="%.16f \n};\n\n";

        else if (i % 4 ==3)

            fmt="%.16f ,\n";

        else fmt="%.16f , ";
        fprintf(fp,fmt,stage_classifier_threshold[i]);
    }

    fprintf(fp,"const int stage_classifier_count[] = \n{\n");

    for( i = 0; i < stage_classifier_count.size(); ++i )
    {
        if (i == stage_classifier_count.size()-1)
            fmt="%d \n};\n\n";

        else fmt="%d, ";
        fprintf(fp,fmt,stage_classifier_count[i]);
    }


    fprintf(fp,"const uchar haar_feature_rect[] = \n{\n");

    for( i = 0; i < haar_feature_rect.size(); ++i )
    {
        if (i == haar_feature_rect.size()-1)
            fmt="%d \n};\n\n";

        else if (i % 40 ==39)

            fmt="%d,\n";

        else fmt="%d,";
        fprintf(fp,fmt,haar_feature_rect[i]);
    }

    fprintf(fp,"const uchar haar_feature_weight[] = \n{\n");

    for( i = 0; i < haar_feature_weight.size(); ++i )
    {
        if (i == haar_feature_weight.size()-1)
            fmt="%d \n};\n\n";

        else if (i % 40 ==39)

            fmt="%d,\n";

        else fmt="%d,";
        fprintf(fp,fmt,cvRound(haar_feature_weight[i]));
    }


    fprintf(fp,"const float classifier_threshold[] = \n{\n");

    for( i = 0; i < classifier_threshold.size(); ++i )
    {
        if (i == classifier_threshold.size()-1)
            fmt="%.16e \n};\n\n";

        else if (i % 4 ==3)

            fmt="%.16e ,\n";

        else fmt="%.16e , ";
        fprintf(fp,fmt,classifier_threshold[i]);
    }


    fprintf(fp,"const uchar classifier_left[] = \n{\n");

    for( i = 0; i < classifier_left.size(); ++i )
    {
        if (i == classifier_left.size()-1)
            fmt="%d \n};\n\n";

        else if (i % 30 ==29)

            fmt="%d,\n";

        else fmt="%d,";
        fprintf(fp,fmt,classifier_left[i]);
    }

    fprintf(fp,"const uchar classifier_right[] = \n{\n");

    for( i = 0; i < classifier_right.size(); ++i )
    {
        if (i == classifier_right.size()-1)
            fmt="%d \n};\n\n";

        else if (i % 30 ==29)

            fmt="%d,\n";

        else fmt="%d,";
        fprintf(fp,fmt,classifier_right[i]);
    }

    fprintf(fp,"const float classifier_alpha[] = \n{\n");

    for( i = 0; i < classifier_alpha.size(); ++i )
    {
        if (i == classifier_alpha.size()-1)
            fmt="%.16e \n};\n\n";

        else if (i % 4 ==3)

            fmt="%.16e ,\n";

        else fmt="%.16e , ";
        fprintf(fp,fmt,classifier_alpha[i]);
    }
    fprintf(fp,"const float classifier_alpha2[] = \n{\n");

    for( i = 0; i < classifier_alpha2.size(); ++i )
    {
        if (i == classifier_alpha2.size()-1)
            fmt="%.16e \n};\n\n";

        else if (i % 4 ==3)

            fmt="%.16e ,\n";

        else fmt="%.16e , ";
        fprintf(fp,fmt,classifier_alpha2[i]);
    }


    fclose(fp);
    return cascade;
}

static CvHaarClassifierCascade*
myCreateHaarClassifierCascade( int stage_count )
{
    CvHaarClassifierCascade* cascade = 0;

    int block_size = sizeof(*cascade) + stage_count*sizeof(*cascade->stage_classifier);

    if( stage_count <= 0 )
        CV_Error( CV_StsOutOfRange, "Number of stages should be positive" );

    cascade = (CvHaarClassifierCascade*)cvAlloc( block_size );
    memset( cascade, 0, block_size );

    cascade->stage_classifier = (CvHaarStageClassifier*)(cascade + 1);
    cascade->flags = CV_HAAR_MAGIC_VAL;
    cascade->count = stage_count;

    return cascade;
}


static CvHaarClassifierCascade*
myLoadHaarClassifierCascade( )
{
    CvHaarClassifierCascade* cascade = NULL;

    int i, j, k, l, n,thresholdcounter, rectcounter, weightcounter,alphacounter,minuscnt ;
    float weightisminus;

    thresholdcounter=0;
    rectcounter=0;
    weightcounter=0;
    alphacounter=0;

    cascade = myCreateHaarClassifierCascade(ClassifierCascadeCount);
    cascade->orig_window_size.width = cascadewidth;
    cascade->orig_window_size.height = cascadeheight;

    for( i = 0; i < ClassifierCascadeCount; ++i )
    {

        cascade->stage_classifier[i].parent = i-1;
        cascade->stage_classifier[i].next = -1;
        cascade->stage_classifier[i].child = i+1;
        cascade->stage_classifier[i].threshold = stage_classifier_threshold[i];

        cascade->stage_classifier[i].count = 0;
        cascade->stage_classifier[i].classifier =
            (CvHaarClassifier*) cvAlloc( stage_classifier_count[i]
                                         * sizeof( cascade->stage_classifier[i].classifier[0] ) );


        cascade->stage_classifier[i].count = stage_classifier_count[i];

        for( j = 0; j < cascade->stage_classifier[i].count; ++j )
            cascade->stage_classifier[i].classifier[j].haar_feature = NULL;

        for( j = 0; j < cascade->stage_classifier[i].count; ++j )
        {

            CvHaarClassifier* classifier =
                &cascade->stage_classifier[i].classifier[j];


            classifier->count = 2;
            classifier->haar_feature = (CvHaarFeature*) cvAlloc(
                                           classifier->count * ( sizeof( *classifier->haar_feature ) +
                                                   sizeof( *classifier->threshold ) +
                                                   sizeof( *classifier->left ) +
                                                   sizeof( *classifier->right ) ) +
                                           (classifier->count + 1) * sizeof( *classifier->alpha ) );
            classifier->threshold = (float*) (classifier->haar_feature+classifier->count);
            classifier->left = (int*) (classifier->threshold + classifier->count);
            classifier->right = (int*) (classifier->left + classifier->count);
            classifier->alpha = (float*) (classifier->right + classifier->count);


            for( k = 0; k < classifier->count; ++k )
            {
                for( l = 0; l < 3; ++l )
                {
                    classifier->haar_feature[k].rect[l].r.x = haar_feature_rect[rectcounter];
                    rectcounter++;
                    classifier->haar_feature[k].rect[l].r.y= haar_feature_rect[rectcounter];
                    rectcounter++;
                    classifier->haar_feature[k].rect[l].r.width= haar_feature_rect[rectcounter];
                    rectcounter++;
                    classifier->haar_feature[k].rect[l].r.height= haar_feature_rect[rectcounter];
                    rectcounter++;

                    weightisminus=haar_feature_weight[weightcounter];
                    weightcounter++;

                    if (weightisminus>4) weightisminus=-1;

                    classifier->haar_feature[k].rect[l].weight=weightisminus ;

                }
                classifier->haar_feature[k].tilted=0;

                classifier->threshold[k] = classifier_threshold[thresholdcounter];

                minuscnt=classifier_left[thresholdcounter];
                if (minuscnt>5) minuscnt-=256;
                classifier->left[k] = minuscnt;

                minuscnt=classifier_right[thresholdcounter];
                if (minuscnt>5) minuscnt-=256;
                classifier->right[k] = minuscnt;

                classifier->alpha[k] =classifier_alpha[thresholdcounter];

                thresholdcounter++;
            }

            classifier->alpha[classifier->count] =classifier_alpha2[alphacounter];
            alphacounter++;

        }
    }

    return cascade;
}
