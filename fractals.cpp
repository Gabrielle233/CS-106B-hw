//Gabrielle Li, CS 106B, Summer 2019
//Assignment 3: Recursion (Part A. Fractals)
//For the first part, I implemented a method to draw Sierpinski Triangle using recursion.
//For the second part, I implemented a method to flood fill an area with one color also using recursion.
//Resources I have used: class lecture slides, Stanford Library, LaIR help with debugging.
//All parts of this assignment was created solely by me.

#include "fractals.h"
#include <math.h>
using namespace std;

//The method takes in a window, a double x and a double y ((x, y) is the cordinate of the point
//which we want to start with to draw the triangle, a double size which represents the size
//of the triangle, and an integer order which represents the desired order of Sierpinski triangle
//to be drawn. This program will make the use of recursion.
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    double sin30 = 0.5;
    double sin60 = sqrt(3)/2;
    //Base case: if the order that is put in is 1, we simply draw a equilateral triangle
    //starting from (x, y) whose side length equal to the size parameter passed in.
    //By calculation using trignometry, the cordinates of the three verticies are:
    //(x, y), (x + size, y), and (x + 1/2 * size, y + sqrt(3)/2 * size).
    if (order == 1) {
        window.drawLine(x, y, x + size, y);
        window.drawLine(x + size, y, x + size * sin30, y + size * sin60);
        window.drawLine(x + size * sin30, y + size * sin60, x, y);
    }

    //Recursive step: in non-base cases, we want to draw three smaller triangles,
    //each of a quarter of the area of the triangle we have now so the new size
    //equals to half of the old size. The position of the smaller triangles are calculated
    //with trignometry, similarly. To do this, we simply call drawSierpinskiTriangle recursively.
    //The order parameter passed in every time when we call the function should decrease by one,
    //so that eventually we will get to the base case.
    else {
        drawSierpinskiTriangle(window, x, y, size/2, order - 1);
        drawSierpinskiTriangle(window, x + size/2, y, size/2, order - 1);
        drawSierpinskiTriangle(window, x + size/4, y + size * sin60 / 2, size/2, order - 1);
    }
}

//This method will floodfill an area with a chosen color. It takes in a window, an integer x
//and an integer y, which represent the cordinate of a point that is in the area we want to
//paint and an integer to resent a color we want to paint with. This program will make the use
//of recursion.
int floodFill(GWindow& window, int x, int y, int color) {
    //Declare an integer count to count the number of pixels we have change the color
    int count = 0;
    //Exception: if the one or both cordinates are negative, throw an error message.
    if ((x < 0) || (y < 0) || (!window.inCanvasBounds(x, y))) {
        throw "The value is not valid.";
    }
    //Base case: Call getPixel to check the color of the selected pixel.
    //If color of the pixel (and then the area) is the same as the color we wish to paint,
    //nothing will be done and count will not be change. Since the base case doesn't do anything,
    //it is not written explicitly.

    //Recursive step: If color of the pixel (and then the area) is different from the color
    //we wish to paint, the program will first record the color it is for future reference and then
    //call setPixel and change the color of that pixel. Every time we change the color for one pixel,
    //count increases by one.
    else if (window.getPixel(x, y) != color) {
        int current = window.getPixel(x, y);
        window.setPixel(x, y, color);
        count ++;
        //Now we go around the selected pixel and change the color of all other adjacent
        //pixels that has the same color (before change) as the pixel. We first check is it's
        //adjacent pixel is in bound. If so, we check if the pixel is of the same color as the
        //selected one. If both are true, we call floodFill on that pixel. If not, we check other
        //pixels adjacent of the selected pixel.
        if (window.inCanvasBounds(x + 1, y) && window.getPixel(x + 1, y) == current) {
            count += floodFill(window, x + 1, y, color);
        }
        if (window.inCanvasBounds(x - 1, y) && window.getPixel(x - 1, y) == current) {
            count += floodFill(window, x - 1, y, color);
        }
        if (window.inCanvasBounds(x, y + 1) && window.getPixel(x, y + 1) == current) {
            count += floodFill(window, x, y + 1, color);
        }
        if (window.inCanvasBounds(x, y - 1) && window.getPixel(x, y - 1) == current) {
            count += floodFill(window, x, y - 1, color);
        }
    }
    //Variable count is returned to show how many pixels have been painted.
    return count;
}
