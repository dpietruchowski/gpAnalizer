#include "terminalset.h"

Mat_ptr terminalSet::inputImage( new Mat(imread("krowaI.png", 0)) );
Mat_ptr terminalSet::nullImage( new Mat(imread("czarny.png", 0)) );

terminalSet::terminalSet()
{
}

/*
 * termina[0] - size
 * terminal[1] - elementType
 * terminal[2] - morphType
 */
/*
int* terminalSet::getRandomTerminal()
{
    int size1, size2, morphType, elementType;

    size1 = 1 + rand() % 15;
    size2 = 1 + rand() % 15;
    morphType = rand() % 5;
    elementType = rand() % 3;

    int* terminal = new int[4];
    terminal[0]=size1;
    terminal[1]=size2;
    terminal[2]=elementType;
    terminal[3]=morphType;
    return terminal;
}
*/
Mat_ptr terminalSet::getRandomImage() const
{
    return this->inputImage;
}

void terminalSet::setInputImage(Mat_ptr inputImage)
{
    this->inputImage = move(inputImage);
}


