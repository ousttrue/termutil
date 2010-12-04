# termutil

Termutil is an terminfo utility for term.js.

This module provide non block stdin input event 
and terminfo command write to stdout.

## build

  node-waf configure build

## usage

  var termutil=require('termutil');
  
  var term=new termutil.Term();
  
  var keymap={
      0x71: // q
          function(){ process.exit(); },
      0x68: // h
          function(){ term.tcmd('cub1'); },
      0x6a: // j
          function(){ term.tcmd('cud1'); },
      0x6b: // k
          function(){ term.tcmd('cuu1'); },
      0x6c: // l
          function(){ term.tcmd('cuf1'); },
  };
  
  term.on('keyinput', function(code){
      if(code in keymap){
          keymap[code]();
      }
      else{
          process.stdout.write('['+code+']');
      }
  });
  
  term.tcmd('clear')

