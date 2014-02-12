#ifndef EVENT_FILTER_H
#define EVENT_FILTER_H


class Event;
typedef void(*handler_t)(Event const &);


class EventFilter
{
public:
  EventFilter();
  ~EventFilter();

  void add(char const * eventName, handler_t callback);
  void handle(Event const &);
  int count();
  char const** events();

private:
  char const* eventNames[64];

  typedef struct {
    char * eventName;
    handler_t callback;
  } filter_t;

  filter_t filters[64];
  int nFilters;
};

#endif

