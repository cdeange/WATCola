#ifndef PARENT_H_
#define PARENT_H_

_Task Parent {
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
    enum State {
      Starting = 'S', 
      Deposit  = 'D', 
      Finished = 'F'
    };

  private:
    void main();

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumStudents;
    unsigned int mDelay;
};

#endif