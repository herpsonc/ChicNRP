/*
 * Post.cpp
 *
 *  Created on: 8 févr. 2020
 *      Author: Valars
 */

#include "Post.h"
#include <iostream>

using namespace std;

Post::Post() {
	id ="";
	time = 0;
}

Post::Post(string id, float time)
{
	this->id = id;
	this->time = time;
}

Post::~Post() {
}

//! \return id id of the Post
const string& Post::getId() const {
	return id;
}

//! \param id id to set
void Post::setId(const string &id) {
	this->id = id;
}

//! \return time working time of the Post
float Post::getTime() const {
	return time;
}

//! \param time set time of the Post
void Post::setTime(float time) {
	this->time = time;
}

//! \return attributs vector of attributs of the Post
const vector<string>& Post::getAttributs() const {
	return attributs;
}

//! \param attributs attributs to set
void Post::setAttributs(const vector<string> &attributs) {
	this->attributs = attributs;
}

//! \param attribut attribut to add
void Post::addAttribut(string attribut) {
	attributs.push_back(attribut);
}
