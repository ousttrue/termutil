var TERMUTIL=require('termutil');

var term=new TERMUTIL.Term();

var app={
    y: 0,
    x: 0,
    lines: 0,
    cols: 0,
    drawed: false,
    stack: [],

    resize: function(size){
        this.lines=size[0]-1;
        this.cols=size[1];
        this.clip();
        this.redraw();
    },
    clip:function(){
        if(this.x<0){
            this.x=0;
        }
        else if(this.x>=this.cols){
            this.x=this.cols-1;
        }
        if(this.y<0){
            this.y=0;
        }
        else if(this.y>=this.lines){
            this.y=this.lines-1;
        }
    },
    left: function(){ 
        this.x-=1;
        this.clip();
        this.redraw();
    },
    down: function(){ 
        this.y+=1;
        this.clip();
        this.redraw();
    },
    up: function(){ 
        this.y-=1;
        this.clip();
        this.redraw();
    },
    right: function(){ 
        this.x+=1;
        this.clip();
        this.redraw();
    },
    lastline: function(){
        this.y=this.lines-1;
        this.redraw();
    },
    firstline: function(){
        this.y=0;
        this.redraw();
    },
    linehead: function(){
        this.x=0;
        this.redraw();
    },
    linetail: function(){
        this.x=this.cols-1;
        this.redraw();
    },
    yx: function(y, x){ 
        term.tcmd('cup', y, x); 
        this.redraw();
    },
    redraw: function(){
        this.drawed=false;
        process.nextTick(function(){ app.draw(); });
    },
    draw: function(){
        if(this.drawed){
            return;
        }
        this.push();
        term.tcmd('cup', this.lines, 0);
        process.stdout.write("lines: "+this.y+"/"+this.lines+", cols: "+this.x+"/"+this.cols);
        term.tcmd('el');
        this.pop();
        this.drawed=true;
    },
    push: function(){
        this.stack.push([this.y, this.x]);
    },
    pop: function(){
        var pos=this.stack.pop();
        this.yx(pos[0], pos[1]);
    }
};

var keymap={
    0x71: // q
        function(){ process.exit(); },
    0x68: // h
        function(){ app.left(); },
    0x6a: // j
        function(){ app.down(); },
    0x6b: // k
        function(){ app.up(); },
    0x6c: // l
        function(){ app.right(); },

    0x47: // G
        function(){ app.lastline(); },
    0x67: // g
        function(){ app.firstline(); },
    0x24: // $
        function(){ app.linetail(); },
    0x30:
        function(){ app.linehead(); },
};

term.on('keyinput', function(code){
    if(code in keymap){
        keymap[code]();
    }
    else{
        //process.stdout.write('['+code+']');
    }
});

process.on('exit', function(){
    var size=term.size();
    term.tcmd('cup', size[0], 0);
    process.stdout.write('\n');
});

process.on('SIGWINCH', function(){
    app.resize(term.size());
});

// initialize
var size=term.size();
term.tcmd('cup', size[0], 0);
for(var i=0; i<size[0]; ++i){
    process.stdout.write('\n');
}
app.resize(size);

term.listen();
