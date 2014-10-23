// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <myCubInterface_IDL.h>
#include <yarp/os/idl/WireTypes.h>



class myCubInterface_IDL_ping : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("ping",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_homeAll : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("homeAll",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_startController : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("startController",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_stopController : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("stopController",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_setPose : public yarp::os::Portable {
public:
  int32_t joint;
  int32_t pos;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeTag("setPose",1,1)) return false;
    if (!writer.writeI32(joint)) return false;
    if (!writer.writeI32(pos)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_getPose : public yarp::os::Portable {
public:
  int32_t joint;
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("getPose",1,1)) return false;
    if (!writer.writeI32(joint)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readI32(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_gotoPose : public yarp::os::Portable {
public:
  int32_t joint;
  int32_t pos;
  int32_t t;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(4)) return false;
    if (!writer.writeTag("gotoPose",1,1)) return false;
    if (!writer.writeI32(joint)) return false;
    if (!writer.writeI32(pos)) return false;
    if (!writer.writeI32(t)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_gotoPoseSync : public yarp::os::Portable {
public:
  int32_t joint;
  int32_t pos;
  int32_t t;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(4)) return false;
    if (!writer.writeTag("gotoPoseSync",1,1)) return false;
    if (!writer.writeI32(joint)) return false;
    if (!writer.writeI32(pos)) return false;
    if (!writer.writeI32(t)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_setPoseAll : public yarp::os::Portable {
public:
  std::vector<int32_t>  poses;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("setPoseAll",1,1)) return false;
    {
      if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(poses.size()))) return false;
      std::vector<int32_t> ::iterator _iter0;
      for (_iter0 = poses.begin(); _iter0 != poses.end(); ++_iter0)
      {
        if (!writer.writeI32((*_iter0))) return false;
      }
      if (!writer.writeListEnd()) return false;
    }
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_getPoseAll : public yarp::os::Portable {
public:
  std::vector<int32_t>  _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getPoseAll",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    {
      _return.clear();
      uint32_t _size1;
      yarp::os::idl::WireState _etype4;
      reader.readListBegin(_etype4, _size1);
      _return.resize(_size1);
      uint32_t _i5;
      for (_i5 = 0; _i5 < _size1; ++_i5)
      {
        if (!reader.readI32(_return[_i5])) {
          reader.fail();
          return false;
        }
      }
      reader.readListEnd();
    }
    return true;
  }
};

class myCubInterface_IDL_gotoPoseAll : public yarp::os::Portable {
public:
  std::vector<int32_t>  poses;
  std::vector<int32_t>  times;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeTag("gotoPoseAll",1,1)) return false;
    {
      if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(poses.size()))) return false;
      std::vector<int32_t> ::iterator _iter6;
      for (_iter6 = poses.begin(); _iter6 != poses.end(); ++_iter6)
      {
        if (!writer.writeI32((*_iter6))) return false;
      }
      if (!writer.writeListEnd()) return false;
    }
    {
      if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(times.size()))) return false;
      std::vector<int32_t> ::iterator _iter7;
      for (_iter7 = times.begin(); _iter7 != times.end(); ++_iter7)
      {
        if (!writer.writeI32((*_iter7))) return false;
      }
      if (!writer.writeListEnd()) return false;
    }
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_getDistance : public yarp::os::Portable {
public:
  int32_t id;
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("getDistance",1,1)) return false;
    if (!writer.writeI32(id)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readI32(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_getOrientation : public yarp::os::Portable {
public:
  std::vector<int32_t>  _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getOrientation",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    {
      _return.clear();
      uint32_t _size8;
      yarp::os::idl::WireState _etype11;
      reader.readListBegin(_etype11, _size8);
      _return.resize(_size8);
      uint32_t _i12;
      for (_i12 = 0; _i12 < _size8; ++_i12)
      {
        if (!reader.readI32(_return[_i12])) {
          reader.fail();
          return false;
        }
      }
      reader.readListEnd();
    }
    return true;
  }
};

class myCubInterface_IDL_getHeading : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getHeading",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readI32(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_getBatteryVolt : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getBatteryVolt",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readI32(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class myCubInterface_IDL_getBatteryCurrent : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getBatteryCurrent",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readI32(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

bool myCubInterface_IDL::ping() {
  bool _return = false;
  myCubInterface_IDL_ping helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::ping()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::homeAll() {
  bool _return = false;
  myCubInterface_IDL_homeAll helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::homeAll()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::startController() {
  bool _return = false;
  myCubInterface_IDL_startController helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::startController()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::stopController() {
  bool _return = false;
  myCubInterface_IDL_stopController helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::stopController()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::setPose(const int32_t joint, const int32_t pos) {
  bool _return = false;
  myCubInterface_IDL_setPose helper;
  helper.joint = joint;
  helper.pos = pos;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::setPose(const int32_t joint, const int32_t pos)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t myCubInterface_IDL::getPose(const int32_t joint) {
  int32_t _return = 0;
  myCubInterface_IDL_getPose helper;
  helper.joint = joint;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t myCubInterface_IDL::getPose(const int32_t joint)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::gotoPose(const int32_t joint, const int32_t pos, const int32_t t) {
  bool _return = false;
  myCubInterface_IDL_gotoPose helper;
  helper.joint = joint;
  helper.pos = pos;
  helper.t = t;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::gotoPose(const int32_t joint, const int32_t pos, const int32_t t)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::gotoPoseSync(const int32_t joint, const int32_t pos, const int32_t t) {
  bool _return = false;
  myCubInterface_IDL_gotoPoseSync helper;
  helper.joint = joint;
  helper.pos = pos;
  helper.t = t;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::gotoPoseSync(const int32_t joint, const int32_t pos, const int32_t t)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::setPoseAll(const std::vector<int32_t> & poses) {
  bool _return = false;
  myCubInterface_IDL_setPoseAll helper;
  helper.poses = poses;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::setPoseAll(const std::vector<int32_t> & poses)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::vector<int32_t>  myCubInterface_IDL::getPoseAll() {
  std::vector<int32_t>  _return;
  myCubInterface_IDL_getPoseAll helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","std::vector<int32_t>  myCubInterface_IDL::getPoseAll()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool myCubInterface_IDL::gotoPoseAll(const std::vector<int32_t> & poses, const std::vector<int32_t> & times) {
  bool _return = false;
  myCubInterface_IDL_gotoPoseAll helper;
  helper.poses = poses;
  helper.times = times;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool myCubInterface_IDL::gotoPoseAll(const std::vector<int32_t> & poses, const std::vector<int32_t> & times)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t myCubInterface_IDL::getDistance(const int32_t id) {
  int32_t _return = 0;
  myCubInterface_IDL_getDistance helper;
  helper.id = id;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t myCubInterface_IDL::getDistance(const int32_t id)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::vector<int32_t>  myCubInterface_IDL::getOrientation() {
  std::vector<int32_t>  _return;
  myCubInterface_IDL_getOrientation helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","std::vector<int32_t>  myCubInterface_IDL::getOrientation()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t myCubInterface_IDL::getHeading() {
  int32_t _return = 0;
  myCubInterface_IDL_getHeading helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t myCubInterface_IDL::getHeading()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t myCubInterface_IDL::getBatteryVolt() {
  int32_t _return = 0;
  myCubInterface_IDL_getBatteryVolt helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t myCubInterface_IDL::getBatteryVolt()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t myCubInterface_IDL::getBatteryCurrent() {
  int32_t _return = 0;
  myCubInterface_IDL_getBatteryCurrent helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t myCubInterface_IDL::getBatteryCurrent()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool myCubInterface_IDL::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  reader.expectAccept();
  if (!reader.readListHeader()) { reader.fail(); return false; }
  yarp::os::ConstString tag = reader.readTag();
  while (!reader.isError()) {
    // TODO: use quick lookup, this is just a test
    if (tag == "ping") {
      bool _return;
      _return = ping();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "homeAll") {
      bool _return;
      _return = homeAll();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "startController") {
      bool _return;
      _return = startController();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "stopController") {
      bool _return;
      _return = stopController();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "setPose") {
      int32_t joint;
      int32_t pos;
      if (!reader.readI32(joint)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(pos)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = setPose(joint,pos);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getPose") {
      int32_t joint;
      if (!reader.readI32(joint)) {
        reader.fail();
        return false;
      }
      int32_t _return;
      _return = getPose(joint);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "gotoPose") {
      int32_t joint;
      int32_t pos;
      int32_t t;
      if (!reader.readI32(joint)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(pos)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(t)) {
        t = 100;
      }
      bool _return;
      _return = gotoPose(joint,pos,t);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "gotoPoseSync") {
      int32_t joint;
      int32_t pos;
      int32_t t;
      if (!reader.readI32(joint)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(pos)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(t)) {
        t = 100;
      }
      bool _return;
      _return = gotoPoseSync(joint,pos,t);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "setPoseAll") {
      std::vector<int32_t>  poses;
      {
        poses.clear();
        uint32_t _size13;
        yarp::os::idl::WireState _etype16;
        reader.readListBegin(_etype16, _size13);
        poses.resize(_size13);
        uint32_t _i17;
        for (_i17 = 0; _i17 < _size13; ++_i17)
        {
          if (!reader.readI32(poses[_i17])) {
            reader.fail();
            return false;
          }
        }
        reader.readListEnd();
      }
      bool _return;
      _return = setPoseAll(poses);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getPoseAll") {
      std::vector<int32_t>  _return;
      _return = getPoseAll();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        {
          if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<int32_t> ::iterator _iter18;
          for (_iter18 = _return.begin(); _iter18 != _return.end(); ++_iter18)
          {
            if (!writer.writeI32((*_iter18))) return false;
          }
          if (!writer.writeListEnd()) return false;
        }
      }
      reader.accept();
      return true;
    }
    if (tag == "gotoPoseAll") {
      std::vector<int32_t>  poses;
      std::vector<int32_t>  times;
      {
        poses.clear();
        uint32_t _size19;
        yarp::os::idl::WireState _etype22;
        reader.readListBegin(_etype22, _size19);
        poses.resize(_size19);
        uint32_t _i23;
        for (_i23 = 0; _i23 < _size19; ++_i23)
        {
          if (!reader.readI32(poses[_i23])) {
            reader.fail();
            return false;
          }
        }
        reader.readListEnd();
      }
      {
        times.clear();
        uint32_t _size24;
        yarp::os::idl::WireState _etype27;
        reader.readListBegin(_etype27, _size24);
        times.resize(_size24);
        uint32_t _i28;
        for (_i28 = 0; _i28 < _size24; ++_i28)
        {
          if (!reader.readI32(times[_i28])) {
            reader.fail();
            return false;
          }
        }
        reader.readListEnd();
      }
      bool _return;
      _return = gotoPoseAll(poses,times);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getDistance") {
      int32_t id;
      if (!reader.readI32(id)) {
        id = 0;
      }
      int32_t _return;
      _return = getDistance(id);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getOrientation") {
      std::vector<int32_t>  _return;
      _return = getOrientation();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        {
          if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<int32_t> ::iterator _iter29;
          for (_iter29 = _return.begin(); _iter29 != _return.end(); ++_iter29)
          {
            if (!writer.writeI32((*_iter29))) return false;
          }
          if (!writer.writeListEnd()) return false;
        }
      }
      reader.accept();
      return true;
    }
    if (tag == "getHeading") {
      int32_t _return;
      _return = getHeading();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getBatteryVolt") {
      int32_t _return;
      _return = getBatteryVolt();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getBatteryCurrent") {
      int32_t _return;
      _return = getBatteryCurrent();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "help") {
      std::string functionName;
      if (!reader.readString(functionName)) {
        functionName = "--all";
      }
      std::vector<std::string> _return=help(functionName);
      yarp::os::idl::WireWriter writer(reader);
        if (!writer.isNull()) {
          if (!writer.writeListHeader(2)) return false;
          if (!writer.writeTag("many",1, 0)) return false;
          if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<std::string> ::iterator _iterHelp;
          for (_iterHelp = _return.begin(); _iterHelp != _return.end(); ++_iterHelp)
          {
            if (!writer.writeString(*_iterHelp)) return false;
           }
          if (!writer.writeListEnd()) return false;
        }
      reader.accept();
      return true;
    }
    if (reader.noMore()) { reader.fail(); return false; }
    yarp::os::ConstString next_tag = reader.readTag();
    if (next_tag=="") break;
    tag = tag + "_" + next_tag;
  }
  return false;
}

std::vector<std::string> myCubInterface_IDL::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("ping");
    helpString.push_back("homeAll");
    helpString.push_back("startController");
    helpString.push_back("stopController");
    helpString.push_back("setPose");
    helpString.push_back("getPose");
    helpString.push_back("gotoPose");
    helpString.push_back("gotoPoseSync");
    helpString.push_back("setPoseAll");
    helpString.push_back("getPoseAll");
    helpString.push_back("gotoPoseAll");
    helpString.push_back("getDistance");
    helpString.push_back("getOrientation");
    helpString.push_back("getHeading");
    helpString.push_back("getBatteryVolt");
    helpString.push_back("getBatteryCurrent");
    helpString.push_back("help");
  }
  else {
    if (functionName=="ping") {
      helpString.push_back("bool ping() ");
      helpString.push_back("ping the control board ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="homeAll") {
      helpString.push_back("bool homeAll() ");
      helpString.push_back("move all joints to the home position ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="startController") {
      helpString.push_back("bool startController() ");
      helpString.push_back("start motor controller ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="stopController") {
      helpString.push_back("bool stopController() ");
      helpString.push_back("stop motor controller ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="setPose") {
      helpString.push_back("bool setPose(const int32_t joint, const int32_t pos) ");
      helpString.push_back("Set joint pos ");
      helpString.push_back("@joint joint id (0..3) ");
      helpString.push_back("@pos   joint pos (0..90) ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getPose") {
      helpString.push_back("int32_t getPose(const int32_t joint) ");
      helpString.push_back("get joint pos ");
      helpString.push_back("@joint joint id (0..3) ");
      helpString.push_back("@return joint pos ");
    }
    if (functionName=="gotoPose") {
      helpString.push_back("bool gotoPose(const int32_t joint, const int32_t pos, const int32_t t = 100) ");
      helpString.push_back("Move a joint to position with time duration ");
      helpString.push_back("@joint joint id (0..3) ");
      helpString.push_back("@pos   joint pos (0..90) ");
      helpString.push_back("@t     time duration in ms ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="gotoPoseSync") {
      helpString.push_back("bool gotoPoseSync(const int32_t joint, const int32_t pos, const int32_t t = 100) ");
      helpString.push_back("Move a joint to position with time duration ");
      helpString.push_back("and wait until it reaches the desired position ");
      helpString.push_back("@joint joint id (0..3) ");
      helpString.push_back("@pos   joint pos (0..90) ");
      helpString.push_back("@t     time duration in ms ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="setPoseAll") {
      helpString.push_back("bool setPoseAll(const std::vector<int32_t> & poses) ");
      helpString.push_back("Set all joint pos ");
      helpString.push_back("@poses a vector of joint poses (must have 4 elements) ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getPoseAll") {
      helpString.push_back("std::vector<int32_t>  getPoseAll() ");
      helpString.push_back("get all joint poses ");
      helpString.push_back("@return joint poses ");
    }
    if (functionName=="gotoPoseAll") {
      helpString.push_back("bool gotoPoseAll(const std::vector<int32_t> & poses, const std::vector<int32_t> & times) ");
      helpString.push_back("move all joint pos with desired duration ");
      helpString.push_back("@poses a vector of joint poses (must have 4 elements) ");
      helpString.push_back("@times a vector of time duration (must have 4 elements) ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getDistance") {
      helpString.push_back("int32_t getDistance(const int32_t id = 0) ");
      helpString.push_back("get range-finder value ");
      helpString.push_back("@id the sensor id started from 0 ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getOrientation") {
      helpString.push_back("std::vector<int32_t>  getOrientation() ");
      helpString.push_back("get digital compass values ");
      helpString.push_back("@returna a vector of x,y,z ");
    }
    if (functionName=="getHeading") {
      helpString.push_back("int32_t getHeading() ");
      helpString.push_back("get absolute heading ");
      helpString.push_back("@return heading angle ");
    }
    if (functionName=="getBatteryVolt") {
      helpString.push_back("int32_t getBatteryVolt() ");
      helpString.push_back("get battery voltage status ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getBatteryCurrent") {
      helpString.push_back("int32_t getBatteryCurrent() ");
      helpString.push_back("get battery current status ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="help") {
      helpString.push_back("std::vector<std::string> help(const std::string& functionName=\"--all\")");
      helpString.push_back("Return list of available commands, or help message for a specific function");
      helpString.push_back("@param functionName name of command for which to get a detailed description. If none or '--all' is provided, print list of available commands");
      helpString.push_back("@return list of strings (one string per line)");
    }
  }
  if ( helpString.empty()) helpString.push_back("Command not found");
  return helpString;
}


