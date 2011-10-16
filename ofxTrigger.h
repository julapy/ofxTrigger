/*
 *  ofxTrigger.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#define TRIGGER_TYPE_INT            0
#define TRIGGER_TYPE_BOOL           1
#define TRIGGER_TYPE_FLOAT          2
#define TRIGGER_TYPE_STRING         3
#define TRIGGER_TYPE_UNDEFINED      4

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxTriggerItem
{
public:
    ofxTriggerItem()
    {
        triggerID       = "";
        triggerType     = TRIGGER_TYPE_UNDEFINED;
        triggerTime     = 0;
        triggerValue    = NULL;
    }
    string  triggerID;
    int     triggerType;
    int     triggerTime;
    void*   triggerValue;
    
    int valueInt ()
    {
        if( triggerType == TRIGGER_TYPE_INT )
            return *( (int*)triggerValue );
        return 0;
    }
    
    bool valueBool ()
    {
        if( triggerType == TRIGGER_TYPE_BOOL )
            return *( (bool*)triggerValue );
        return false;
    }
    
    float valueFloat ()
    {
        if( triggerType == TRIGGER_TYPE_FLOAT )
            return *( (float*)triggerValue );
        return 0.0;
    }
    
    string valueString ()
    {
        if( triggerType == TRIGGER_TYPE_STRING )
            return *( (string*)triggerValue );
        return "";
    }
};

class ofxTrigger
{
public :
	 ofxTrigger();
	~ofxTrigger();

    void addTrigger ( string triggerID, int    triggerValue, int time = ofGetElapsedTimeMillis() );
    void addTrigger ( string triggerID, bool   triggerValue, int time = ofGetElapsedTimeMillis() );
    void addTrigger ( string triggerID, float  triggerValue, int time = ofGetElapsedTimeMillis() );    
    void addTrigger ( string triggerID, string triggerValue, int time = ofGetElapsedTimeMillis() );
    void addTrigger ( string triggerID, void*  triggerValue, int triggerType, int time = ofGetElapsedTimeMillis() );
    
    void addTriggerHook ( string triggerID, string* triggerPtr );
    void addTriggerHook ( string triggerID, float*  triggerPtr );
    void addTriggerHook ( string triggerID, int*    triggerPtr );
    void addTriggerHook ( string triggerID, bool*   triggerPtr );
    void addTriggerHook ( string triggerID, void*   triggerPtr, int triggerType );
    
    void setStartTimeMillis ( int time = ofGetElapsedTimeMillis() );
    void jumpToTimeMillis   ( int time );
	
    vector<ofxTriggerItem>&
         triggersAtTimeInMillis ( int time = ofGetElapsedTimeMillis() );
    
    void removeAllTriggers      ();
    void removeTriggersBetween  ( int startTime, int endTime );
    void removeTriggerValue     ( ofxTriggerItem& trigger );
    void reset ();
    
    void save   ( string fileName = "triggers.xml" );
    void load   ( string fileName = "triggers.xml" );
	
    vector<ofxTriggerItem> triggers;
    vector<ofxTriggerItem> triggersPtr;
    vector<ofxTriggerItem> triggersOnFrame;
	int	triggerIndex;
    int triggerTime;
    int triggerStartTime;
};
