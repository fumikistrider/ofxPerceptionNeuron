#pragma once

#include "ofMain.h"

class ofxBvh;

class ofxBvhJoint
{
	friend class ofxBvh;
	
public:
	
	enum CHANNEL
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};
	
	ofxBvhJoint(string name, ofxBvhJoint *parent) : name(name),  parent(parent), index(-1) {}
	
	inline const string& getName() const { return name; }
	inline const ofVec3f& getOffset() const { return offset; }
	
	inline const ofMatrix4x4& getMatrix() const { return matrix; }
	inline const ofMatrix4x4& getGlobalMatrix() const { return global_matrix; }
	
	inline ofVec3f getPosition() const { return global_matrix.getTranslation(); }
	inline ofQuaternion getRotate() const { return global_matrix.getRotate(); }
	
	inline ofxBvhJoint* getParent() const { return parent; }
	inline const vector<ofxBvhJoint*>& getChildren() const { return children; }

	inline bool isSite() const { return children.empty(); }
	inline bool isRoot() const { return !parent; }
	
	inline ofxBvh* getBvh() const { return bvh; }
	
protected:
    int index;

	string name;
	ofVec3f initial_offset;
	ofVec3f offset;
	
	ofMatrix4x4 matrix;
	ofMatrix4x4 global_matrix;
	
	ofxBvh* bvh;
	
	vector<ofxBvhJoint*> children;
	ofxBvhJoint* parent;
	
	vector<CHANNEL> channel_type;
};

class ofxBvh
{
public:
	
	ofxBvh(const string& data) : root(NULL), total_channels(0), rate(1), loop(false),
		playing(false), play_head(0), need_update(false)
    {
        load(data);
    }
	
	virtual ~ofxBvh();
	
    void update(const vector<float>& data);
	void draw();
		
	const int getNumJoints() const { return joints.size(); }
	const ofxBvhJoint* getJoint(int index);
	const ofxBvhJoint* getJoint(string name);
protected:
    void load(const string& data);
    void unload();
	
	typedef vector<float> FrameData;
	
	int total_channels;
	
	ofxBvhJoint* root;
	vector<ofxBvhJoint*> joints;
	map<string, ofxBvhJoint*> jointMap;
	
	vector<FrameData> frames;
	FrameData currentFrame;
	
	int num_frames;
	float frame_time;
	
	float rate;
	
	bool playing;
	float play_head;
	
	bool loop;
	bool need_update;
	bool frame_new;
	
	void parseHierarchy(const string& data);
	ofxBvhJoint* parseJoint(int& index, vector<string> &tokens, ofxBvhJoint *parent);
	void updateJoint(int& index, const FrameData& frame_data, ofxBvhJoint *joint);
	
};