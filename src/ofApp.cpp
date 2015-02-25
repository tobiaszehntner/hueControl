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
    
    // Testing redirects and https.
    std::string url = "http://192.168.100.100/api/tobiaszehntner/lights/";
    
    
    ofx::HTTP::BaseResponse response;
    ofx::HTTP::GetRequest request(url);
    
    hueBridgeIP = "192.168.100.100";
    hueUser = "tobiaszehntner";
    
    hueLightNum = "1";
    hueGroupNum = "1";
    
    std::string putUrlGroup = "http://" + hueBridgeIP + "/api/" + hueUser + "/groups/" + hueGroupNum + "/action";
    std::string putUrlSingle = "http://" + hueBridgeIP + "/api/" + hueUser + "/lights/" + hueLightNum + "/state";
    
    
    
    
    Json::Value messageBody;
    messageBody["on"]             = true;
    messageBody["bri"]            = 254;     // 1-254 (254 brightest)
    messageBody["hue"]            = 0;       // 0-65535 (red to red)
    messageBody["sat"]            = 254;     // 0-254 (0 = white)
    messageBody["alert"]          = "none";  // none, select, lselect
    messageBody["effect"]         = "none";  // "colorloop" or none
    messageBody["transitiontime"] = 10;      // 10 = 1sec;
    messageBody["ct"]             = 500;     // 153 (6500K/daylight) - 500 (2000K/candlelight)
    
    ofBuffer bodyBuffer = messageBody.toStyledString();
    
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
void ofApp::draw(){
 
}

