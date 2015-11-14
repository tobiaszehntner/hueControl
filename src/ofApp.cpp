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


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Hue Setup
    hueBridgeIP = "192.168.100.100";
    hueUser = "tobiaszehntner";
    hueGetObject = "lights"; // "", "lights", "groups", "config"
    hueLightNum = "1";
    hueGroupNum = "2";
    hueCTcold = true;
    counter = 1;
    
    ctCold = 200; // 153-500
    ctWarm = 450; // 153-500
    transitionTime = 5*600; // 10 = 1sec, 600 = 1min
    offsetTime = 0*1000;
    
    hueChangingParameter = "ct"; // on(true/false), bri(1-254), hue(0-65535), sat(0-254), xy(0-1), ct(153-500), alert(none,select,lselect), effect(none, colorloop)
    hueChangingParameter1 = 153;
    hueChangingParameter2 = 500; //0-60000// hue from 0 to 10000 (yellow to red)
    
    // HTTP Setup
    getUrl       = "http://" + hueBridgeIP + "/api/" + hueUser + "/" + hueGetObject;
    putUrlGroup  = "http://" + hueBridgeIP + "/api/" + hueUser + "/groups/" + hueGroupNum + "/action";
    putUrlSingle = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + hueLightNum + "/state";
    
    // Timer Setup
    timer1Interval = transitionTime*100 + 1000;
    timer1.setup(timer1Interval);
    timer1.start(true) ;
    ofAddListener( timer1.TIMER_COMPLETE , this, &ofApp::timer1CompleteHandler );
    ofAddListener( timer1.TIMER_STARTED , this, &ofApp::timer1StartedHandler );
    
    timer2.setup(1*60000);
    timer2.start(true) ;
    ofAddListener( timer2.TIMER_COMPLETE , this, &ofApp::timer2CompleteHandler );
    ofAddListener( timer2.TIMER_STARTED , this, &ofApp::timer2StartedHandler );
    
    
    // GET request
    ofx::HTTP::GetRequest getRequest(getUrl);
    
    // Print GET request
    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = client.execute(getRequest,
                                                      response,
                                                      context);
        
        // Request and response headers can be examined here.
        
        
        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        
        // Flush the input stream.
        std::cout << std::endl;
        
        std::cout << "============" << endl;
        
        
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }
    
    
    // Setting lights to daylight
    Json::Value messageBody;
    messageBody["on"]             = true;
    messageBody["bri"]            = 254;     // 1-254 (254 brightest)
    messageBody["hue"]            = 0;       // 0-65535 (red to red)
    messageBody["sat"]            = 254;     // 0-254 (0 = white)
    messageBody["alert"]          = "none";  // none, select, lselect
    messageBody["effect"]         = "none";  // "colorloop" or none
    messageBody["transitiontime"] = 0;      // 10 = 1sec;
    messageBody["ct"]             = ctCold;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
    
    messageBody["transitiontime"]      = transitionTime;      // 10 = 1sec;
    messageBody[hueChangingParameter]  = hueChangingParameter1;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
    
    bodyBuffer = messageBody.toStyledString();
    
    cout << bodyBuffer.getText() << endl;
   
    
    ofx::HTTP::PutRequest putRequest(putUrlGroup);
    putRequest.setPutBuffer(bodyBuffer);
    
    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = client.execute(putRequest,
                                                      response,
                                                      context);
        
        // Request and response headers can be examined here.
        
        
        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        
        // Flush the input stream.
        std::cout << std::endl;
        
        std::cout << "============" << endl;
        
        
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    timer1.update( ) ;
    timer2.update( ) ;
    
}

void ofApp::timer1CompleteHandler( int &args )
{
    // what to do when timer ends
    cout<<"TIMER1 ENDED"<<endl;
    
    Json::Value messageBody;
    
    
    
    
    if (hueCTcold) {
        // Setting lights to warm light
        messageBody["transitiontime"] = transitionTime;      // 10 = 1sec;
        messageBody[hueChangingParameter]             = hueChangingParameter2;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
        
        /////
        
        for (int i = 1; i < 10; i++) {
            
            hueLightNum = ofToString(i,0);
            putUrlSingle = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + hueLightNum + "/state";
            
            bodyBuffer = messageBody.toStyledString();
            cout << "Light No "<< hueLightNum << endl;
            cout << bodyBuffer.getText() << endl;
            ofx::HTTP::PutRequest putRequest(putUrlSingle);
            putRequest.setPutBuffer(bodyBuffer);
            
            try
            {
                // Execute the request and get the response stream.
                std::istream& responseStream = client.execute(putRequest,
                                                              response,
                                                              context);
                
                // Request and response headers can be examined here.
                
                
                std::cout << "============" << endl;
                // Copy the output to the terminal.
                Poco::StreamCopier::copyStream(responseStream, std::cout);
                
                // Flush the input stream.
                std::cout << std::endl;
                
                std::cout << "============" << endl;
                
                
            }
            catch(const Poco::Exception& exc)
            {
                ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
            }
            catch(...)
            {
                ofLogError("ofApp::setup") << "Got unknown exception.";
            }
            
            ofSleepMillis(offsetTime);
        }
        
        timer1.start(true);
        hueCTcold = false;
        
    } else {
        // Setting lights to cold light
        messageBody["transitiontime"] = transitionTime;      // 10 = 1sec;
        messageBody[hueChangingParameter]             = hueChangingParameter1;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
        
        for (int i = 1; i < 10; i++) {
            
            hueLightNum = ofToString(i,0);
            putUrlSingle = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + hueLightNum + "/state";
            
            bodyBuffer = messageBody.toStyledString();
            cout << "Light No "<< hueLightNum << endl;
            cout << bodyBuffer.getText() << endl;
            ofx::HTTP::PutRequest putRequest(putUrlSingle);
            putRequest.setPutBuffer(bodyBuffer);
            
            try
            {
                // Execute the request and get the response stream.
                std::istream& responseStream = client.execute(putRequest,
                                                              response,
                                                              context);
                
                // Request and response headers can be examined here.
                
                
                std::cout << "============" << endl;
                // Copy the output to the terminal.
                Poco::StreamCopier::copyStream(responseStream, std::cout);
                
                // Flush the input stream.
                std::cout << std::endl;
                
                std::cout << "============" << endl;
                
                
            }
            catch(const Poco::Exception& exc)
            {
                ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
            }
            catch(...)
            {
                ofLogError("ofApp::setup") << "Got unknown exception.";
            }
            
            ofSleepMillis(offsetTime);
        }
        
        timer1.start(true);
        hueCTcold = true;
    }
    
    
}

void ofApp::timer1StartedHandler( int &args ) {
    // what to do when timer starts
}

void ofApp::timer2CompleteHandler( int &args )
{
//    std::cout << "Runtime " << counter%(timer1Interval/1000/60) << " mins; Loop " << timer1Interval/1000/60 << "mins; Offset: " << offsetTime/1000 << " secs" << endl;
//    counter++;
//    timer2.start(true);
    
}

void ofApp::timer2StartedHandler( int &args ) {
    // what to do when timer starts
}

