// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once

#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxJSON.h"
#include "ofxSimpleTimer.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
        void update();

    std::string hueBridgeIP;
    std::string hueUser;
    std::string hueGetObject;
    std::string hueLightNum;
    std::string hueGroupNum;
    bool hueCTcold;
    int ctCold;
    int ctWarm;
    int transitionTime;
    int offsetTime;
    
    int counter;
    int timer1Interval;
    
    std::string hueChangingParameter;
    int hueChangingParameter1;
    int hueChangingParameter2;
    
    std::string getUrl;
    std::string putUrlGroup;
    std::string putUrlSingle;

    ofx::HTTP::DefaultClient client;
    ofx::HTTP::Context context;
    ofx::HTTP::BaseResponse response;
    
    ofBuffer bodyBuffer;
    
    
    ofxSimpleTimer timer1;
    void timer1CompleteHandler( int &args );
    void timer1StartedHandler( int &args );
    
    ofxSimpleTimer timer2;
    void timer2CompleteHandler( int &args );
    void timer2StartedHandler( int &args );
    
};
