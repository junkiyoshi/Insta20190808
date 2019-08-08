#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(3);
	ofNoFill();
	ofEnableDepthTest();

	this->font_size = 300;
	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);

	for (char c = 'A'; c <= 'Z'; c++) {

		this->chara_path.push_back(font.getCharacterAsPoints(c, true, false));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(180);
	ofRotateY(180);

	int span = 380;
	for (int x = -span; x <= span; x += span) {

		for (int y = -span; y <= span; y += span) {

			ofPushMatrix();
			ofTranslate(x, y);

			int char_index = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0005), 0, 1, 0, this->chara_path.size());
			auto outline = this->chara_path[char_index].getOutline();
			for (int line_index = 0; line_index < outline.size(); line_index++) {

				outline[line_index] = outline[line_index].getResampledByCount(80);
				auto vertices = outline[line_index].getVertices();
				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

					auto point = vertices[vertices_index] + glm::vec2(this->font_size * -0.5, this->font_size * 0.5);
					auto next_point = vertices[vertices_index + 1] + glm::vec2(this->font_size * -0.5, this->font_size * 0.5);
					if (vertices_index == vertices.size() - 1) {

						point = vertices[vertices_index] + glm::vec2(this->font_size * -0.5, this->font_size * 0.5);
						next_point = vertices[0] + glm::vec2(this->font_size * -0.5, this->font_size * 0.5);
					}
					auto z = ofMap(ofNoise((x + point.x) * 0.005, (y + point.y) * 0.005, ofGetFrameNum() * 0.005), 0, 1, 0, -200);

					ofFill();
					ofSetColor(239);

					ofBeginShape();
					ofVertex(point);
					ofVertex(next_point);
					ofVertex(next_point + glm::vec3(0, 0, z));
					ofVertex(point + glm::vec3(0, 0, z));
					ofEndShape(true);

					ofNoFill();
					ofSetColor(39);

					ofBeginShape();
					ofVertex(point);
					ofVertex(next_point);
					ofVertex(next_point + glm::vec3(0, 0, z));
					ofVertex(point + glm::vec3(0, 0, z));
					ofEndShape(true);
				}
			}

			ofPopMatrix();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}