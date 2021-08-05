#pragma once

class Observer
{
public:
    virtual void FileUpdate() = 0;
    virtual void ModeUpdate() = 0;
    virtual void FilenameUpdate() = 0;
    virtual void StringUpdate() = 0;
};

class Observable
{
private:
    Observer* observer_;

public:
    void AddObserver(Observer* observer)
    {
        observer_ = observer;
    }
    void NotifyFileUpdate()
    {
        observer_->FileUpdate();
    }
    void NotifyModeUpdate()
    {
        observer_->ModeUpdate();
    }
    void NotifyFilenameUpdate()
    {
        observer_->FilenameUpdate();
    }
    void NotifyStringUpdate()
    {
        observer_->StringUpdate();
    }
};

