//Author: Nico Ruan
//Disjoint Algorithm with BMP
//CS415

#include "EasyBMP.h"
#include <list>
#include <iostream>
#include <string>

using namespace std;

class PixVal{
public:
  PixVal* parent;//keeps track of parent node
  int RedVal;//keeps track of RedVal (could have used Blue or Green as well)
  int size;//size of the set that it is in
  bool root;//keeps track of whether or not it is a root node
  bool isValid;//keeps track of whether or not it is a valid pixel
  
};

void Union(PixVal* pix1, PixVal* pix2);
PixVal* Find(PixVal* pixel);

int main(int argc, char* argv[]){
  
  if(argc != 2){
    cout << "Error: Make sure you give a BMP file when trying to run" << endl;
    return 1;
  }
  BMP picture;
  picture.ReadFromFile(argv[1]);
  
  //turns all pixels to black and white
  for(int j=0; j< picture.TellHeight(); j++){
    for (int i=0; i< picture.TellWidth(); i++){
      if((int) picture(i,j)->Red < 250 && (int) picture(i,j)-> Green < 250
         && picture(i,j)->Blue < 250){
        RGBApixel Temp = picture.GetPixel(i,j);
        Temp.Red = 0;
        Temp.Green = 0;
        Temp.Blue = 0;
        Temp.Alpha = 0;
        picture.SetPixel(i,j,Temp);
      }
    }
  }

  //initialze array to the size of the image
  PixVal* PixelArray[picture.TellHeight()][picture.TellWidth()];

  for(int j=0; j< picture.TellHeight(); j++){
    for (int i=0; i< picture.TellWidth(); i++){
      //initializes everything for each pixel;
      PixVal* pixel= new PixVal;
      pixel->size = 1;
      pixel->root = true;
      pixel->isValid = true;
      pixel->RedVal = picture(i,j)->Red;
      pixel->parent = pixel;
      PixelArray[j][i] = pixel;
    }
  }

  //Makes all white pixels invalid so that we never really look at them
  for(int j=0; j< picture.TellHeight(); j++){
    for (int i=0; i< picture.TellWidth(); i++){
      if(PixelArray[j][i]->RedVal > 10 ){
	PixelArray[j][i]->isValid = false;
      }
    }
  }

  for(int j=0; j< picture.TellHeight(); j++){
    for (int i=0; i< picture.TellWidth(); i++){
      if(PixelArray[j][i]->isValid == true){
	//Union find calling to creat the sets of pixels
	PixVal* original = PixelArray[j][i];
        PixVal* temp1 = PixelArray[j][i-1];
        PixVal* temp2= PixelArray[j-1][i];
        PixVal* temp3 = PixelArray[j+1][i];
        PixVal* temp4 = PixelArray[j][i+1];

	if(PixelArray[j-1][i]->isValid == true && Find(original) != Find(temp2)){//up
	  Union(PixelArray[j-1][i],PixelArray[j][i]);
        }
	if(PixelArray[j+1][i]->isValid == true && Find(original) != Find(temp3)){//down
          Union(PixelArray[j+1][i],PixelArray[j][i]);
        }
	if (PixelArray[j][i-1]->isValid == true && Find(original) != Find(temp1)){//left
          Union(PixelArray[j][i-1], PixelArray[j][i]);
        }
	if(PixelArray[j][i+1]->isValid == true && Find(original) != Find(temp4)){//right
          Union(PixelArray[j][i+1], PixelArray[j][i]);
        }
      }
    }
  }

  //this will calculate the number of letters and mark the root of each letter in red
  int numletters = 0;
  for(int j=0; j< picture.TellHeight(); j++){
    for (int i=0; i< picture.TellWidth(); i++){
      if(PixelArray[j][i]->isValid == true){
	if(PixelArray[j][i]->parent == Find(PixelArray[j][i]) && PixelArray[j][i]->root == true){
	  numletters+=1;
	  picture(i,j)->Red = 250;
	  picture(i,j)->Green = 0;
	  picture(i,j)->Blue = 0;
	  picture(i,j)->Alpha = 0;
	}
      }
    }
  }
  
  picture.WriteToFile("counted.bmp");//marks the root or parent node of each letter
  cout << "The number of letters in the image is: " << numletters << endl;
  return 0;
}

void Union(PixVal* pix1, PixVal* pix2){
  //joins two pixels together assigning the one in the smaller group to that of the 
  //bigger one

  PixVal* temp1 = Find(pix1);
  PixVal* temp2 = Find(pix2);
  if (temp1 == temp2)
    return;
  if (temp1->size > temp2->size){
    pix2->parent = temp1;
    temp1->size += temp2->size;
    pix2->root = false;
    temp2->root = false;
  }//end if

  else{
    pix1->parent = temp2;
    temp2->size += temp1->size;
    pix1->root = false;
    temp1->root = false;
  }
}//end function

PixVal* Find(PixVal* pixel){
  //finds the parent of the pixel passed into it and returns said parent
  if(pixel->parent == pixel)
    return pixel;
  else 
    return Find(pixel->parent);
}//end Find()

