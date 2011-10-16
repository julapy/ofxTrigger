/*
 *  ofxTrigger.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTrigger.h"

ofxTrigger :: ofxTrigger()
{
	reset();
    setStartTimeMillis( 0 );
}

ofxTrigger :: ~ofxTrigger()
{
    removeAllTriggers();
}

//////////////////////////////////////////////////////
//  ADD TRIGGERS.
//////////////////////////////////////////////////////

void ofxTrigger :: addTrigger( string triggerID, int triggerValue, int time )
{
    addTrigger( triggerID, &triggerValue, TRIGGER_TYPE_INT, time );
}

void ofxTrigger :: addTrigger( string triggerID, bool triggerValue, int time )
{
    addTrigger( triggerID, &triggerValue, TRIGGER_TYPE_BOOL, time );
}

void ofxTrigger :: addTrigger( string triggerID, float triggerValue, int time )
{
    addTrigger( triggerID, &triggerValue, TRIGGER_TYPE_FLOAT, time );
}

void ofxTrigger :: addTrigger( string triggerID, string triggerValue, int time )
{
    addTrigger( triggerID, &triggerValue, TRIGGER_TYPE_STRING, time );
}

void ofxTrigger :: addTrigger( string triggerID, void* triggerValue, int triggerType, int time )
{
    ofxTriggerItem item;
    item.triggerID      = triggerID;
    item.triggerType    = triggerType;
    item.triggerTime    = time;
    
    if( triggerType == TRIGGER_TYPE_INT )
    {
        int* intValue       = new int();
        *intValue           = *( (int*)triggerValue );
        item.triggerValue   = intValue;
    }
    else if( triggerType == TRIGGER_TYPE_BOOL )
    {
        int* boolValue      = new int();
        *boolValue          = *( (bool*)triggerValue );
        item.triggerValue   = boolValue;
    }
    else if( triggerType == TRIGGER_TYPE_FLOAT )
    {
        float* floatValue   = new float();
        *floatValue         = *( (float*)triggerValue );
        item.triggerValue   = floatValue;
    }
    else if( triggerType == TRIGGER_TYPE_STRING )
    {
        string* stringValue = new string();
        *stringValue        = *( (string*)triggerValue );
        item.triggerValue   = stringValue;
    }
    
    //------------------------------------------------------- place item in correct order, ascending.
    bool bAdded = false;
    for( int i=0; i<triggers.size(); i++ )
    {
        if( item.triggerTime < triggers[ i ].triggerTime )
        {
            triggers.insert( triggers.begin() + i, item );
            bAdded = true;
            break;
        }
    }
    
    if( !bAdded )
        triggers.push_back( item );
}

//////////////////////////////////////////////////////
//  ADD TRIGGER POINTERS.
//////////////////////////////////////////////////////

void ofxTrigger :: addTriggerHook( string triggerID, int* triggerPtr )
{
    addTriggerHook( triggerID, triggerPtr, TRIGGER_TYPE_INT );
}

void ofxTrigger :: addTriggerHook( string triggerID, bool* triggerPtr )
{
    addTriggerHook( triggerID, triggerPtr, TRIGGER_TYPE_BOOL );
}

void ofxTrigger :: addTriggerHook( string triggerID, float* triggerPtr )
{
    addTriggerHook( triggerID, triggerPtr, TRIGGER_TYPE_FLOAT );
}

void ofxTrigger :: addTriggerHook( string triggerID, string* triggerPtr )
{
    addTriggerHook( triggerID, triggerPtr, TRIGGER_TYPE_STRING );
}

void ofxTrigger :: addTriggerHook ( string triggerID, void* triggerPtr, int triggerType )
{
    ofxTriggerItem item;
    item.triggerID      = triggerID;
    item.triggerValue   = triggerPtr;
    item.triggerType    = triggerType;

    triggersPtr.push_back( item );
}

void ofxTrigger :: removeAllTriggers ()
{
	for( int i=0; i<triggers.size(); i++ )
        removeTriggerValue( triggers[ i ] );
    
    triggers.clear();
}

void ofxTrigger :: removeTriggersBetween  ( int startTime, int endTime )
{
    if( startTime >= endTime )
        return;
    
    int i = 0;
    int t = triggers.size();
	for( i; i<t; i++ )
    {
        int time = triggers[ i ].triggerTime;
        
        if( time >= startTime && time <= endTime  )
        {
            removeTriggerValue( triggers[ i ] );
            triggers.erase( triggers.begin() + i );
            
            --i;
            --t;
        }
        
        if( time > endTime )       // all items forward of this item are in the future.
            break;
    }
}

void ofxTrigger :: removeTriggerValue ( ofxTriggerItem& trigger )
{
    int triggerType;
    triggerType = trigger.triggerType;
    
    if( triggerType == TRIGGER_TYPE_INT )
    {
        delete (int*)trigger.triggerValue;
    }
    else if( triggerType == TRIGGER_TYPE_BOOL )
    {
        delete (bool*)trigger.triggerValue;
    }
    else if( triggerType == TRIGGER_TYPE_FLOAT )
    {
        delete (float*)trigger.triggerValue;
    }
    else if( triggerType == TRIGGER_TYPE_STRING )
    {
        delete (string*)trigger.triggerValue;
    }
}

void ofxTrigger :: reset ()
{
    triggerTime  = 0;
    triggerIndex = 0;
}

void ofxTrigger :: setStartTimeMillis ( int time )
{
    triggerStartTime = time;
}

void ofxTrigger :: jumpToTimeMillis ( int time )
{
    setStartTimeMillis();
    
    triggerStartTime -= time;
    triggerTime       = time;
    triggerIndex      = 0;
}

vector<ofxTriggerItem>& ofxTrigger :: triggersAtTimeInMillis ( int time )
{
    time -= triggerStartTime;
    
    triggersOnFrame.clear();

    int i = triggerIndex;
    int t = triggers.size();
    
    for( i; i<t; i++ )
    {
        const ofxTriggerItem& item = triggers[ i ];
        
        if( item.triggerTime > triggerTime && item.triggerTime <= time )
        {
            triggersOnFrame.push_back( item );
            triggerIndex = i + 1;
        }
        
        if( item.triggerTime > time )       // all items forward of this item are in the future.
            break;
    }
    
    triggerTime = time;

    return triggersOnFrame;
}

void ofxTrigger :: save ( string fileName )
{
    ofxXmlSettings xml;
    xml.addTag( "triggers" );
    xml.pushTag( "triggers" );
    
    for( int i=0; i<triggers.size(); i++ )
    {
        ofxTriggerItem& item = triggers[ i ];
        
        xml.addTag( "trigger" );
        
        xml.addAttribute( "trigger", "id",      item.triggerID,     i );
        xml.addAttribute( "trigger", "type",    item.triggerType,   i );
        xml.addAttribute( "trigger", "time",    item.triggerTime,   i );
        
        int triggerType;
        triggerType = triggers[ i ].triggerType;
        
        if( triggerType == TRIGGER_TYPE_INT )
        {
            xml.addAttribute( "trigger", "value", *((int*)item.triggerValue), i );
        }
        else if( triggerType == TRIGGER_TYPE_BOOL )
        {
            xml.addAttribute( "trigger", "value", *((bool*)item.triggerValue), i );
        }
        else if( triggerType == TRIGGER_TYPE_FLOAT )
        {
            xml.addAttribute( "trigger", "value", *((float*)item.triggerValue), i );
        }
        else if( triggerType == TRIGGER_TYPE_STRING )
        {
            xml.addAttribute( "trigger", "value", *((string*)item.triggerValue), i );
        }
    }
    
    xml.popTag();
    xml.saveFile( fileName );
}

void ofxTrigger :: load ( string fileName )
{
    removeAllTriggers();

    ofxXmlSettings xml;
    xml.loadFile( fileName );
    xml.pushTag( "triggers" );
    
    int t = xml.getNumTags( "trigger" );
    for( int i=0; i<t; i++ )
    {
        ofxTriggerItem item;
        item.triggerID      = xml.getAttribute( "trigger", "id",     "", i );
        item.triggerType    = xml.getAttribute( "trigger", "type",   TRIGGER_TYPE_UNDEFINED, i );
        item.triggerTime    = xml.getAttribute( "trigger", "time",   0, i );
        
        if( item.triggerType == TRIGGER_TYPE_INT )
        {
            int* intValue       = new int();
            *intValue           = xml.getAttribute( "trigger", "value", 0, i );
            item.triggerValue   = intValue;
        }
        else if( item.triggerType == TRIGGER_TYPE_BOOL )
        {
            int* boolValue      = new int();
            *boolValue          = xml.getAttribute( "trigger", "value", 0, i );
            item.triggerValue   = boolValue;
        }
        else if( item.triggerType == TRIGGER_TYPE_FLOAT )
        {
            float* floatValue   = new float();
            *floatValue         = xml.getAttribute( "trigger", "value", 0, i );
            item.triggerValue   = floatValue;
        }
        else if( item.triggerType == TRIGGER_TYPE_STRING )
        {
            string* stringValue = new string();
            *stringValue        = xml.getAttribute( "trigger", "value", "", i );
            item.triggerValue   = stringValue;
        }
        
        triggers.push_back( item );
    }
    
    xml.popTag();
}
