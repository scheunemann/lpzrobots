/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 *   $Log$
 *   Revision 1.2  2006-07-14 12:24:02  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.3  2006/03/31 16:18:32  fhesse
 *   tracing in oderagent via trackrobots
 *
 *   Revision 1.1.2.2  2006/03/30 12:33:15  fhesse
 *   trace via trackrobot
 *
 *   Revision 1.1.2.1  2005/11/16 11:24:27  martius
 *   moved to selforg
 *
 *   Revision 1.4  2005/11/10 09:08:26  martius
 *   trace has a name
 *
 *   Revision 1.3  2005/11/09 13:31:51  martius
 *   GPL'ised
 *
 ***************************************************************************/
#ifndef __TRACKROBOTS_H
#define __TRACKROBOTS_H

#include <stdio.h>
#include <string.h>

class AbstractRobot;
class Agent;

class TrackRobot {
public:

  friend class Agent;
  //friend class OdeAgent;

  TrackRobot(){
    trackPos = false;
    trackSpeed = false;
    trackOrientation = false;
    tracePos = false;
    interval = 1;
    file=0;
    cnt=0;
    scene=0;
  }
  /** Set the tracking mode of the simulation environment. 
      If one of the trackparameters is true the tracking is enabled.
      The tracking is written into a file with the current date and time as name.
      If tracePos is ture (at least in ode simulations) the trace of the robot is shown
   */ 
  TrackRobot(bool trackPos, bool trackSpeed, bool trackOrientation, bool tracePos, 
	     const char* scene, int interval = 1){
    this->trackPos     = trackPos;
    this->trackSpeed   = trackSpeed;
    this->trackOrientation = trackOrientation;
    this->tracePos     = tracePos;
    this->interval = interval;
    this->scene = strdup(scene);
    file=0;
    cnt=0;
  }

  ~TrackRobot(){
    if(scene) free(scene);
  }

  bool open(const AbstractRobot* robot);
  void track(AbstractRobot* robot);
  void close();

  //todo: do this with friend class OdeAgent or the like
  bool trace() const {return tracePos;};

 private:
  bool trackPos;
  bool trackSpeed;
  bool trackOrientation;
  bool tracePos;

  int interval;
  FILE* file;
  char* scene;
  long cnt;
};



#endif