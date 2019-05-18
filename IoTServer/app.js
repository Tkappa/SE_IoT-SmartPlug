var express = require('express');
var app = express();
var path=require('path');

var r=0;
var b=0;
var g=0;

var bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({
	   extended: false
}));

app.listen(8000, function () {
	  console.log('Example app listening on port 8000!');
});


app.get('/',function(req,res){
	res.send("SP+"+r+";"+b+";"+g+";-E");
	console.log("omg"+ new Date().getHours() + ":" + new Date().getMinutes() + ":" + new Date().getSeconds());
});

app.post('/postc',function(req,res){
	var color=req.body.favcolor;
	var hehe=hexToRgb(color);
	console.log(hehe);
	r=hehe.r;
	g=hehe.g;
	b=hehe.b;
	res.redirect("/");
});

app.post('/recv',function(req,res){
	var values=req.body.values;
	var id=req.body.id;
	var key=req.body.key;

	console.log(id);
	console.log(key);
	var helo=values.split(",");
	console.log(helo);
	console.log("Hehe :^)"+ new Date().getHours() + ":" + new Date().getMinutes() + ":" + new Date().getSeconds());
	res.sendStatus(200);
});

app.get('/color',function(req,res){
	res.sendFile(path.join(__dirname+'/color.html'));
});


function rgbToHex(r, g, b) {
	    return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1);
}


function hexToRgb(hex) {
	var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
	    hex = hex.replace(shorthandRegex, function(m, r, g, b) {
		            return r + r + g + g + b + b;
		        });

	    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
	    return result ? {
		            r: parseInt(result[1], 16),
		            g: parseInt(result[2], 16),
		            b: parseInt(result[3], 16)
		        } : null;
}
