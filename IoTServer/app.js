var express = require('express');
var app = express();
var path=require('path');
var fs = require('fs');
var bodyParser = require('body-parser');
app.use(express.static(path.join(__dirname,'static')));
app.use(bodyParser.urlencoded({
	   extended: false
}));

//Global variables
var G_maxhousewattage=0;
var G_fakereading=0;

var G_devices=[
	//{name:"Placeholder name",id:"01",key:"4242",currcommand:"",currcommandid:0,lastcommandrecieved:1,lastreading:0,tounpair:0}
];
var G_lastcommandid=1000;




app.listen(8000, function () {
	  console.log('The server is up and running on port 8000!');
});

//Index of the application
app.get('/',function(req,res){
	res.sendFile("/content/index.html",  {root: __dirname});
});

//Used by the front end to send commands
app.post('/postc',function(req,res){
	var devid=req.body.id;
	var command=req.body.command;
	var currentdevice =  G_devices.find(function(element) {
	  return element.id = devid;
	});
	switch (command) {
		case "turnoff":
		currentdevice.currcommandid=G_lastcommandid;
		currentdevice.currcommand="SP+MM="+G_lastcommandid+";0;-";
		currentdevice.lastcommandrecieved=0;

		break;
		case "turnon":
		currentdevice.currcommandid=G_lastcommandid;
		currentdevice.currcommand="SP+MM="+G_lastcommandid+";1;-";
		currentdevice.lastcommandrecieved=0;
		break;
		case "setwattage":
		var wattage=req.body.wattage;
		currentdevice.currcommandid=G_lastcommandid;
		currentdevice.currcommand="SP+DSW="+G_lastcommandid+";"+wattage+";-";
		currentdevice.lastcommandrecieved=0;
		break;
		case "sendtime":

		var date = new Date();
		var time = (date.getHours()*60+date.getMinutes())*60+date.getSeconds();
		currentdevice.currcommandid=G_lastcommandid;
		currentdevice.currcommand="SP+TM="+G_lastcommandid+";"+time+";-";
		currentdevice.lastcommandrecieved=0;
		break;
		case "unpair":
		currentdevice.currcommandid=G_lastcommandid;
		currentdevice.currcommand="SP+SUN="+G_lastcommandid+";-";
		currentdevice.lastcommandrecieved=0;
		currentdevice.tounpair=1;
		break;
		case "setroutine":
		var from1=req.body.from1;
		var from2=req.body.from2;
		var to1=req.body.to1;
		var to2=req.body.to2;
		console.log(from1+"-"+to1+","+from2+"-"+to2);
		break;
		case "setname":
		var name=req.body.name;
		currentdevice.name=name;
		break;
		default:

	}
	G_lastcommandid=G_lastcommandid+1;
	if(G_lastcommandid>9999){
		G_lastcommandid=1000;
	}
	res.redirect("/");
});

//Used by devices to get commands
app.get('/cmd',function(req,res){

	var devid=req.query.id;
	var devkey=req.query.key;
	var currentdevice =  G_devices.find(function(element) {
	  return (element.id == devid) && (element.key == devkey) ;
	});
	if(currentdevice==undefined){
		res.send("SP+SUN=1000;-");
		return;
	}
	//If this is the unpair command delete the device from the list
	if(currentdevice.tounpair==1){
		var index = G_devices.indexOf(currentdevice);
		if (index > -1) {
		  G_devices.splice(index, 1);
		}
	}
	console.log(currentdevice.id+" has recieved command "+ currentdevice.currcommand+ " with id "+ currentdevice.currcommandid)
	res.send(currentdevice.currcommand);
});

//Used by the devices to send values
app.post('/recv',function(req,res){
	var values=req.body.values;
	var values=values.split(",");
	var id=req.body.id;
	var key=req.body.key;
	var currentdevice =  G_devices.find(function(element) {
		return (element.id == id) && (element.key == key) ;
	});
	if(currentdevice==undefined){
		res.sendStatus(404);
		return;
	}

	//The last value in the array is the most recent , so we reverse it;
	values.reverse();
	currentdevice.lastreading=values[0];

	//Checks if this value brings triggers the max house wattage
	if(G_maxhousewattage>0){
		var currentreading=0;
		G_devices.forEach(function(element) {
	  	currentreading=currentreading+ parseInt(element.lastreading);
		});
		currentreading= currentreading +parseInt(G_fakereading);
		if(currentreading>G_maxhousewattage){
			currentdevice.currcommandid=G_lastcommandid;
			currentdevice.currcommand="SP+MM="+G_lastcommandid+";0;-";
			currentdevice.lastcommandrecieved=0;
			G_lastcommandid=G_lastcommandid+1;
		}
	}

	//Writes the new reading into the json
	var devdata;
	fs.readFile(path.join(__dirname,"/data/"+id+"_"+key+".json"),function(err,data){
		if (err){
			res.sendStatus(404);
			return;
		}
		devdata=JSON.parse(data);
		var currDate= new Date();

		values.forEach(function(element){
			devdata.readings.push({date:currDate.toString(),reading: element});
			currDate.setSeconds(currDate.getSeconds()-5);
		});
		fs.writeFile(path.join(__dirname,"/data/"+id+"_"+key+".json"), JSON.stringify(devdata), function (err, file) {
		  if (err) throw err;
		});
		res.sendStatus(200);
	});

});

//Used by the devices to confirm they recieved a command
app.post('/confirmcmd',function(req,res){
	var commandconfirm=req.body.cmd;
	var id=req.body.id;
	var key=req.body.key;
	var currentdevice =  G_devices.find(function(element) {
	  return (element.id == id) && (element.key == key) && (element.currcommandid ==commandconfirm) ;
	});
	if(currentdevice==undefined){
		res.sendStatus(404);
		return;
	}

	console.log("id : "+ id+"con chiave "+key+" ha confermato il ricevimento del comando  "+commandconfirm);
	currentdevice.lastcommandrecieved=1;
	res.sendStatus(200);
});

//Used by the user to post the general settings
app.post('/postsett',function(req,res){
	var wattage=req.body.wattage;
	var command=req.body.command;
	switch(command){
		case "setfakereading":
		G_fakereading=wattage;
		break;
		case "setmaxwatt":
		G_maxhousewattage=wattage;
		break;
	};
	res.redirect("/");
});

//Used by the front page to populate the data
app.get('/settings',function(req,res){
	var currentreading=0;
	var deviceinfo=[];
	G_devices.forEach(function(element) {
  	currentreading=currentreading+ parseInt(element.lastreading) ;
		deviceinfo.push({
			name: element.name,
			id: element.id,
			commrecv: element.lastcommandrecieved
		});
	});
	currentreading= currentreading +parseInt(G_fakereading);
	res.json({settings:{
		reading : currentreading,
		G_fakereading: G_fakereading,
		maxhouse : G_maxhousewattage,
		deviceinfo : deviceinfo
	}
	})
});

//Used by the user to see the readings of a device
app.get('/reading',function(req,res){
	var id=req.query.id;
	var key;
	var currentdevice =  G_devices.find(function(element) {
	  return (element.id == id) ;
	});
	key=currentdevice.key;
	if(currentdevice==undefined){
		res.sendStatus(404);
		return;
	}
	res.cookie('data', JSON.stringify({id:id}));
	res.sendFile("/static/readings.html",  {root: __dirname});
});

//Used by the reading page to populate all the table readings
app.get('/tablereading',function(req,res){
	var id=req.query.id;
	var key;
	var currentdevice =  G_devices.find(function(element) {
	  return (element.id == id) ;
	});
	key=currentdevice.key;
	if(currentdevice==undefined){
		res.sendStatus(404);
		return;
	}
	fs.readFile(path.join(__dirname,"/data/"+id+"_"+key+".json"),function(err,data){
		if (err){
			res.sendStatus(404);
			return;
		}
		var readings=JSON.parse(data);
		res.json(readings);
	});
});

//Used by the App to request a new device to be added
app.get('/newdevice',function(req,res){
	var id = G_devices.length;
	if(id>=99){
		res.sendStatus(404);
		return;
	}
	id=parseInt(id)+1;

	//Writes the id as a string
	if(id<=9){
		id="0"+id;
	}
	else{
		id=""+id;
	}

	//Generates a random key
	key = Math.floor(Math.random() * (+9999 - +1000)) + +1000;
	key=""+key;

	//adds the new device to the array
	var currdev= {
		name:"dev"+id,
		id:id,
		key:key,
		currcommand:"",
		currcommandid:0,
		lastcommandrecieved:1,
		lastreading:0,
		tounpair:0
	};
	G_devices.push(currdev)

	//Creates the devices json file
	fs.writeFile(path.join(__dirname,"/data/"+id+"_"+key+".json"), JSON.stringify({id:id,key:key,readings:[]}), function (err, file) {
	  if (err) throw err;
	});

	//Sends the created device to the app
	res.json({
		id:id,
		key:key
	})
});
