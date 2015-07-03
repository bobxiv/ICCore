#pragma once

// Generates file named 'NormalCircle' inside local dir
// with points over a circle equispaced
// In the range: x -> [-1,1]		y -> [-1,1]
//Params:
//		- N the number of samples
void NormalCircle(int N);

// Generates file named 'NormalT' inside local dir
// with points over a T with its transversal and
// vertical sector 0.5 width.
// In the range: x -> [-1,1]		y -> [-1,1]
//Params:
//		- N the number of samples
void NormalT(int N);