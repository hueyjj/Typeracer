// TextProcessor.h

#ifndef _TEXT_PROCESSOR_H_INCLUDE_

#define _TEXT_PROCESSOR_H_

/* Processes the text and holds all the information for the next words */
class TextProcessor{
 private:
  char *buffer;
  char* formatBuffer(char *fb, int fbLength);
  queue<string> split(char *arr, char delim);

 public:
  queue<string> storage;
  TextProcessor();
  TextProcessor(const char *file);
  ~TextProcessor();
  void loadFile(const char *file);
  char* getBuffer();
  void printAll();
};

#endif
