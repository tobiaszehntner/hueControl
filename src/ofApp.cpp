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
    
    // Timer Setup
    timer1.setup(10000);
    timer1.start(true) ;
    ofAddListener( timer1.TIMER_COMPLETE , this, &ofApp::timer1CompleteHandler );
    ofAddListener( timer1.TIMER_STARTED , this, &ofApp::timer1StartedHandler ) ;
    
    // Hue Setup
    hueBridgeIP = "192.168.100.100";
    hueUser = "tobiaszehntner";
    hueGetObject = "lights"; // "", "lights", "groups", "config"
    hueLightNum = "1";
    hueGroupNum = "1";
    hueCTcold = true;
    
    // HTTP Setup
    getUrl       = "http://" + hueBridgeIP + "/api/" + hueUser + "/" + hueGetObject;
    putUrlGroup  = "http://" + hueBridgeIP + "/api/" + hueUser + "/groups/" + hueGroupNum + "/action";
    putUrlSingle = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + hueLightNum + "/state";
    
    
    
    
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
    messageBody["on"]             = true;
    messageBody["bri"]            = 254;     // 1-254 (254 brightest)
    messageBody["hue"]            = 0;       // 0-65535 (red to red)
    messageBody["sat"]            = 0;     // 0-254 (0 = white)
    messageBody["alert"]          = "none";  // none, select, lselect
    messageBody["effect"]         = "none";  // "colorloop" or none
    messageBody["transitiontime"] = 50;      // 10 = 1sec;
    messageBody["ct"]             = 153;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
    
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
    
}

void ofApp::timer1CompleteHandler( int &args )
{
    // what to do when timer ends
    cout<<"TIMER1 ENDED"<<endl;
    
    if (hueCTcold) {
        // Setting lights to warm light
        messageBody["on"]             = true;
        messageBody["bri"]            = 254;     // 1-254 (254 brightest)
        messageBody["hue"]            = 0;       // 0-65535 (red to red)
        messageBody["sat"]            = 0;     // 0-254 (0 = white)
        messageBody["alert"]          = "none";  // none, select, lselect
        messageBody["effect"]         = "none";  // "colorloop" or none
        messageBody["transitiontime"] = 50;      // 10 = 1sec;
        messageBody["ct"]             = 500;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
        
        hueCTcold = false;
        
    } else {
        // Setting lights to cold light
        messageBody["on"]             = true;
        messageBody["bri"]            = 254;     // 1-254 (254 brightest)
        messageBody["hue"]            = 0;       // 0-65535 (red to red)
        messageBody["sat"]            = 0;     // 0-254 (0 = white)
        messageBody["alert"]          = "none";  // none, select, lselect
        messageBody["effect"]         = "none";  // "colorloop" or none
        messageBody["transitiontime"] = 50;      // 10 = 1sec;
        messageBody["ct"]             = 153;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
        
        hueCTcold = true;
    }
    
    
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
    
    
    timer1.start(true);
}

void ofApp::timer1StartedHandler( int &args ) {
    // what to do when timer starts
}
