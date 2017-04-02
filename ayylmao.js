// ==UserScript==
// @name         Reddit Place Autopainter
// @namespace    http://tampermonkey.net/
// @version      0.2.4
// @description  Try very hard to draw the Arch logo on the reddit place.  This version does random selection.
// @author       MagneticDuck
// @match        https://www.reddit.com/place?webview=true
// @grant        unsafeWindow
// ==/UserScript==

(function() {
    'use strict';

    var delay_ms = 30 * 1000;

    setTimeout(start, 3000);

    function start() {
        var request = new XMLHttpRequest();
        request.open("GET", "/api/me.json");
        request.send();
        request.onload = function(){
            if(this.status != 200) alert("Authentication error");
            var data = JSON.parse(this.response);
            withModhash(data.data.modhash);
        };
    }

    function paintTile(modhash, tile) {
        var request = new XMLHttpRequest();
        request.open("POST", "/api/place/draw.json");
        request.setRequestHeader("X-Modhash", modhash);
        request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        var data = "x=" + tile.x + "&y=" + tile.y + "&color=" + tile.color;
        request.send(data);
    }

    function getTileAt(x, y) {
        var colors = [
            {r: 255, g: 255, b: 255},
            {r: 228, g: 228, b: 228},
            {r: 136, g: 136, b: 136},
            {r: 34, g: 34, b: 34},
            {r: 255, g: 167, b: 209},
            {r: 229, g: 0, b: 0},
            {r: 229, g: 149, b: 0},
            {r: 160, g: 106, b: 66},
            {r: 229, g: 217, b: 0},
            {r: 148, g: 224, b: 68},
            {r: 2, g: 190, b: 1},
            {r: 0, g: 211, b: 221},
            {r: 0, g: 131, b: 199},
            {r: 0, g: 0, b: 234},
            {r: 207, g: 110, b: 228},
            {r: 130, g: 0, b: 128}
        ];
        var data = document.getElementById("place-canvasse").getContext("2d").getImageData(x, y, 1, 1).data;
        return colors.findIndex(function(x) {return JSON.stringify(x) == JSON.stringify({r: data[0], g: data[1], b: data[2]});});
    }

    function danmarkGoal() {
        var dannebro = [
	 //v (752, 295)            
          "________________..__________________________________________________________________",//117
          "_______________.xx._________________________________________________________________",//118
          "_______________.xx.________.......o...............o....xx...x.................______",//119
          "______________.xxxx._______.ooo...o.oo..ooo...oo..o.....x......xxx..x..x.x..x.______",//120
          "______________.xxxx._______.o..o.o....o.o..o.o..o.o.o...x..xx..x..x.x..x.x..x.______",//121
          "_____________.xxxxxx.______.o....o..ooo.o....o....oo.o..x...x..x..x.x..x..xx..______",//122
          "_____________.xxxxxx.______.o...o..o..o.o....o..o.o..o..x...x..x..x.x..x.x..x.______",//123
          "____________.xxxxxxxx._____.o...o...ooo.o.....oo..o..o.xxx.xxx.x..x..xx..x..x.______",//124
          "____________..xxxxxxx._____...................................................______",//125
          "____________...xxxxxxx.____xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx______",//126
          "___________.x...xxxxxx._____________________________________________________________",//127
          "__________.xxx...xxxxxx.____________________________________________________________",//128
          "__________.xxxxx..xxxxx.______________",//129
          "_________.xxxxxxxxxxxxxx._____________",//130
          "_________.xxxxxxxxxxxxxx._____________",//131
          "________.xxxxxxxxxxxxxxxx.____________",//132
          "________.xxxxxxxxxxxxxxxx.____________",//133
          "   ____.xxxxxxxxxxxxxxxxxx.___________",//134
          "   ____.xxxxxxxxxxxxxxxxxx.___________",//135
          "   ___.xxxxxxxxx..xxxxxxxxx.__________",//136
          "   ___.xxxxxxxx....xxxxxxxx.__________",//137
          "   __.xxxxxxxxx....xxxxxxxxx._________",//138
          "   __.xxxxxxxxx....xxxx..xxx._________",//139
          "   _.xxxxxxxxx......xxxx...xx.________",//140
          "   _.xxxxxxxxx......xxxxx.....________",//141
          "   .xxxxxxxxxx......xxxxxxx...._______",//142
          "   .xxxxxxxxxx......xxxxxxxxx.._______",//143
          "  .xxxxxxxxxxx......xxxxxxxxxxx.______",//144
          "  .xxxxxxxxx..______..xxxxxxxxx.______",//145
          " .xxxxxxxx..__________..xxxxxxxx._____",//146
          " .xxxxxx..______________..xxxxxx._____",//147
          ".xxxxx..__________________..xxxxx.____",//148
          ".xxx..______________________..xxx.____",//149
        ];
        var goal = [];
        for (var i = 0; i < dannebro.length; ++i) {
            var line = dannebro[i];
            for (var j = 0; j < dannebro.length; ++j) {
                var color = -1;

                if (line[j] == 'o') color = 0;
                if (line[j] == '_') color = 3;
                if (line[j] == '.') color = 3;
                if (line[j] == 'x') color = 11;

                if (color != -1)
                    goal.push({x: j + 752, y: i + 295, color: color});
            }
        }

        return goal;
    }

    function withModhash(modhash) {
        var goal = danmarkGoal();

        console.log("Søger efter destination ...");

        var paintable = [];
        for (var i = 0; i < goal.length; ++i) {
            var target = goal[i];
            if (getTileAt(target.x, target.y) != target.color) {
                paintable.push({x: target.x, y: target.y, color: target.color});
            }
        }

        console.log(`behøver maling: ${paintable.length}/${goal.length} pixler (${((1-paintable.length/goal.length)*100).toFixed(1)}% korrekt)`);
        
        for(let i = 0; i < 10; i++) {
           if (paintable.length > 0) {
              const inx = Math.floor(Math.random() * paintable.length);
              var picked = paintable[inx];

              console.log("Bruger destination - maler farve: " + JSON.stringify(picked));

              paintTile(modhash, picked);
              paintable.splice(inx, 1);
              
              console.log(`left: ${paintable.length}`);
          }
        }
        
        setTimeout(function() {
            location.reload();
        }, delay_ms);
    }
})();