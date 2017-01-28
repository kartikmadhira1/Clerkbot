#ifndef _ROS_robot_ticks_h
#define _ROS_robot_ticks_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace robot
{

  class ticks : public ros::Msg
  {
    public:
      float right_wheel;
      float left_wheel;

    ticks():
      right_wheel(0),
      left_wheel(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_right_wheel;
      u_right_wheel.real = this->right_wheel;
      *(outbuffer + offset + 0) = (u_right_wheel.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_wheel.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_right_wheel.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_right_wheel.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->right_wheel);
      union {
        float real;
        uint32_t base;
      } u_left_wheel;
      u_left_wheel.real = this->left_wheel;
      *(outbuffer + offset + 0) = (u_left_wheel.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_wheel.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_left_wheel.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_left_wheel.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->left_wheel);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_right_wheel;
      u_right_wheel.base = 0;
      u_right_wheel.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_wheel.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_right_wheel.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_right_wheel.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->right_wheel = u_right_wheel.real;
      offset += sizeof(this->right_wheel);
      union {
        float real;
        uint32_t base;
      } u_left_wheel;
      u_left_wheel.base = 0;
      u_left_wheel.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_wheel.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_left_wheel.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_left_wheel.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->left_wheel = u_left_wheel.real;
      offset += sizeof(this->left_wheel);
     return offset;
    }

    const char * getType(){ return "robot/ticks"; };
    const char * getMD5(){ return "c8a0d6fe18087fd03841f81076bfc858"; };

  };

}
#endif