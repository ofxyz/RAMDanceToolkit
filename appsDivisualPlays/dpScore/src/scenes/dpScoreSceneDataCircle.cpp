//
//  dpScoreSceneDataCircle.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/15/15.
//
//

#include "dpScoreSceneDataCircle.h"

DP_SCORE_NAMESPACE_BEGIN

SceneDataCircle::Circle::Circle()
{
    data.assign(kResolution, 0.f);
}

void SceneDataCircle::Circle::update(float f)
{
    data.push_back(clamp(f));
    while (data.size() > kResolution) {
        data.pop_front();
    }
}

void SceneDataCircle::Circle::draw()
{
    const float step = mRadius * TWO_PI / kResolution;
    const int spacing{30};
    for (int i=0; i<data.size(); i++) {
        if (data.size()>=kResolution-spacing && i>=kResolution-spacing) break;
        const float f{data.at(i)};
        if (::fabsf(f) < 0.01f) continue;
        
        ofPushMatrix();
        const float angle{-360.f / kResolution * (i+spacing/2)};
        ofRotate(angle);
        ofTranslate(0.f, -mRadius);
        const float h{f * mRadius};
        ofFill();
        ofSetColor(128, 128);
        ofRect(-step*0.5f, 0.f, step, h);
        ofNoFill();
        ofSetLineWidth(2.f);
        ofSetColor(ofColor::white, 128);
        ofRect(-step*0.5f, 0.f, step, h);
        ofPopMatrix();
    }
    ofSetLineWidth(step*0.25f);
    ofSetColor(color::kMain, 255);
    ofPushMatrix();
    const float angle{360.f / kResolution * spacing/2};
    ofRotate(angle);
    ofLine(0.f, -mRadius-50.f, 0.f, -mRadius+50.f);
    ofPopMatrix();
    
}

#pragma mark ___________________________________________________________________
void SceneDataCircle::initialize()
{
    dpDebugFunc();
    
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getShortName());
    mUICanvas->addLabel(getShortName(), OFX_UI_FONT_SMALL);
    mUICanvas->addSpacer();
}

void SceneDataCircle::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneDataCircle::enter()
{
    dpDebugFunc();
    
    mCircles.assign(kNumCircles, ofPtr<Circle>());
    for (auto& p : mCircles) {
        p = ofPtr<Circle>(new Circle());
    }
}

void SceneDataCircle::exit()
{
    dpDebugFunc();
    mCircles.clear();
}

void SceneDataCircle::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCameraUnitVectorTotal) {
        if (m.getNumArgs() >= ofVec2f::DIM) {
            ofVec2f v(m.getArgAsFloat(0), m.getArgAsFloat(1));
            mCircles.at(0)->update(v.x);
            mCircles.at(1)->update(v.y);
        }
    }
}

void SceneDataCircle::draw()
{
    ofPushMatrix();
    ofTranslate(kW * 0.5f, kH * 0.5f);
    for (int i=0; i<mCircles.size(); i++) {
        mCircles.at(i)->draw();
    }
    ofPopMatrix();
}

DP_SCORE_NAMESPACE_END