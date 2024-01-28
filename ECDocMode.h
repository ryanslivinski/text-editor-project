#ifndef ECDocMode_h
#define ECDocMode_h
/*
    Edit mode (default) = 0
    Search mode = 1
    Replace mode = 2
*/
enum MODE
{
    EDIT_MODE = 0,
    SEARCH_MODE = 1,
    REPLACE_MODE = 2
};

class ECDocumentMode
{

public:
    ~ECDocumentMode() {}
    ECDocumentMode()
    {
        _mode = EDIT_MODE;
    }

    void SetMode(int x)
    {
        _mode = x;
    }

    int GetMode()
    {
        return _mode;
    }
private:
    int _mode;
};

#endif