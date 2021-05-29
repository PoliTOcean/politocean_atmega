#ifndef EXCEPTION_H
#define EXCEPTION_H

class exception {
  const char *_msg;

public:
  exception() : _msg(nullptr) {}
  exception(const char *msg) : _msg(msg) {}

  const char *what() const { return _msg; }
};

namespace politocean::exception {
class SensorException : public exception {
  const char *_msg;

public:
  SensorException(const char *msg) : _msg(msg) {}

  const char *what() const { return _msg; }
};
} // namespace politocean::exception

#endif // EXCEPTION_H