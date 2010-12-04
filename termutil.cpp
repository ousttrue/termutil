#include "term_wrap.h"

#include <v8.h>
#include <node.h>
#include <node_events.h>


static v8::Persistent<v8::String> keyinput_symbol;


/**
 * Termios & Terminfo wrapper
 */
class Term: node::EventEmitter
{
    ev_io read_watcher_;

public:
    Term()
    {
        ev_init(&read_watcher_, io_event);
        read_watcher_.data=this;
        ev_io_set(&read_watcher_, fileno(stdin), EV_READ);
        ev_io_start(EV_DEFAULT_ &read_watcher_);
    }

    static v8::Handle<v8::Value> New(const v8::Arguments& args)
    {
        //HandleScope scope;
        Term *term=new Term;
        term->Wrap(args.This());
        return args.This();
    }

    static v8::Handle<v8::Value> tcmd(const v8::Arguments &args)
    {
        //HandleScope scope;

        char buf[1024];
        switch(args.Length()){
            case 1:
                {
                    v8::Local<v8::String> cmd=v8::Local<v8::String>::Cast(args[0]);
                    cmd->WriteAscii(buf, 0);
                    Terminfo::tcmd(buf);
                }
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            default:
                break;
        }
    }

private:
    static void io_event (EV_P_ ev_io *w, int revents) 
    {
        Term *term = static_cast<Term*>(w->data);
        if (revents & EV_READ) {
            //HandleScope scope;

            int c;
            while ((c=getchar())!=ERR){
                v8::Local<v8::Value> values[1] = v8::Integer::New(c);
                term->Emit(keyinput_symbol, 1, values);
            }
        }
    }
};


void AddonInitialize(v8::Handle<v8::Object> target)
{
    using namespace v8;
    //HandleScope scope;

    static Termios termios;
    termios.no_canonical();
    Terminfo::initialize();

    keyinput_symbol = NODE_PSYMBOL("keyinput");

    // javascript class Term 
    Local<FunctionTemplate> t = FunctionTemplate::New(Term::New);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->Inherit(node::EventEmitter::constructor_template);
    NODE_SET_PROTOTYPE_METHOD(t, "tcmd", Term::tcmd);
    target->Set(String::NewSymbol("Term"), t->GetFunction());
}

NODE_MODULE(termutil, AddonInitialize);
