

/**
 *  some constants and global vars
 */
var mycub_address = "192.168.1.133";
var mycub_ports = new Object(); // or just {}
mycub_ports['/mycub'] = "10000";
mycub_ports['/MyCubInterface/cmd:i'] = "";
mycub_ports['/GazeControl/cmd:i'] = "";

var isConnected = false;
var canvas_front;
var canvas_rear;
var canvas_3d;


//$(function () {
$(document).ready(function(){
    
    $("#menu_main").zinoMenu({
        enable: function (event, ui) {
            //console.log("enable", event, ui);
        },
        disable: function (event, ui) {
            //console.log("disable", event, ui);
        }
    });

    $("#spinner_j1").zinoSpinner({
        max: 2000,
        min: 0,
        decimal: true,
        step: 100
    });
   var pbar = $("#slider_j1").zinoSlider({
	    bounds: [1],
	    maxValue: 70,
	    slide: function (event, ui) {
            moveJoint(0, ui.data.rightX, $("#spinner_j1").val());
	    }
    });

    $("#spinner_j2").zinoSpinner({
        max: 2000,
        min: 0,
        decimal: true,
        step: 100
    });

    $("#slider_j2").zinoSlider({
	    bounds: [1],
	    maxValue: 70,
	    slide: function (event, ui) {
            moveJoint(1, ui.data.rightX, $("#spinner_j2").val());
	    }
    });

    $("#spinner_j3").zinoSpinner({
        max: 2000,
        min: 0,
        decimal: true,
        step: 100
    });

    $("#slider_j3").zinoSlider({
	    bounds: [1],
	    maxValue: 70,
	    slide: function (event, ui) {
            moveJoint(2, ui.data.rightX, $("#spinner_j3").val());
	    }
    });

    $("#spinner_j4").zinoSpinner({
        max: 2000,
        min: 0,
        decimal: true,
        step: 100
    });

    $("#slider_j4").zinoSlider({
	    bounds: [1],
	    maxValue: 70,
	    slide: function (event, ui) {
            moveJoint(3, ui.data.rightX, $("#spinner_j4").val());
	    }
    });

    $("#spinner_h1").zinoSpinner({
        max: 2000,
        min: 0,
        decimal: true,
        step: 100
    });
    $("#slider_h1").zinoSlider({
	    bounds: [105],
	    maxValue: 180,
	    slide: function (event, ui) {
		    $("#value_h1").val(ui.data.rightX);
            moveCamJoint(ui.data.rightX, $("#value_h2").val(), 200);
	    }
    });

    $("#spinner_h2").zinoSpinner({
        max: 2000,
        min: 0,
        decimal: true,
        step: 100
    });

    $("#slider_h2").zinoSlider({
	    bounds: [105],
	    maxValue: 180,
	    slide: function (event, ui) {
		    $("#value_h2").val(ui.data.rightX);
            moveCamJoint($("#value_h1").val(), ui.data.rightX, 200);
	    }
    });

    $( "#btnminos_j1" ).click(function() {      
      moveJoint(0, $("#value_j1").val() - 5, $("#spinner_j1").val());
    });

    $( "#btnplus_j1" ).click(function() {  
      moveJoint(0, parseInt($("#value_j1").val()) + 5, $("#spinner_j1").val());
    });

    $( "#btnminos_j2" ).click(function() {      
      moveJoint(1, $("#value_j2").val() - 5, $("#spinner_j2").val());
    });

    $( "#btnplus_j2" ).click(function() {      
      moveJoint(1, parseInt($("#value_j2").val()) + 5, $("#spinner_j2").val());
    });

    $( "#btnminos_j3" ).click(function() {      
        moveJoint(2, $("#value_j3").val() - 5, $("#spinner_j3").val());
    });

    $( "#btnplus_j3" ).click(function() {      
      moveJoint(2, parseInt($("#value_j3").val()) + 5, $("#spinner_j3").val());
    });

    $( "#btnminos_j4" ).click(function() {      
      moveJoint(3, $("#value_j4").val() - 5, $("#spinner_j4").val());
    });

    $( "#btnplus_j4" ).click(function() {      
        moveJoint(3, parseInt($("#value_j4").val()) + 5, $("#spinner_j4").val());
    });


    $( "#btnminos_h1" ).click(function() {      
        moveCamJoint($("#value_h1").val() - 5, $("#value_h2").val(), 200);
        $("#value_h1").val($("#value_h1").val() - 5);
    });

    $( "#btnplus_h1" ).click(function() {  
        moveCamJoint(parseInt($("#value_h1").val()) + 5, $("#value_h2").val(), 200);
        $("#value_h1").val(parseInt($("#value_h1").val()) + 5);
    });

    $( "#btnminos_h2" ).click(function() {      
        moveCamJoint($("#value_h1").val() , $("#value_h2").val() - 5, 200);
        $("#value_h2").val($("#value_h2").val() - 5);
    });

    $( "#btnplus_h2" ).click(function() {  
        moveCamJoint($("#value_h1").val() , parseInt($("#value_h2").val()) + 5, 200);
        $("#value_h2").val(parseInt($("#value_h2").val()) + 5);
    });

    canvas_front = zino.Canvas({
        target: document.getElementById("canvas_front"),
        width: 160,
        height: 70
    });

    canvas_rear = zino.Canvas({
        target: document.getElementById("canvas_rear"),
        width: 160,
        height: 70
    });

    /*
    canvas_top = zino.Canvas({
        target: document.getElementById("canvas_top"),
        width: 320,
        height: 290
    });
    */

    drawFront(canvas_front, 0, 0);
    drawFront(canvas_rear, 0, 0);

    resolvesAdress(mycub_address, mycub_ports);

    onloadHandler()

    setInterval(function() {
        updateConnectionStatus();
        updateAllStatus();        
    }, 1000);

});

function drawFront(canvas, j1, j2) {
    var x1 = 50 * Math.cos((90+j1)*Math.PI/180);
    var y1 = 50 * Math.sin((90+j1)*Math.PI/180);
    var x2 = 50 * Math.cos((90-j2)*Math.PI/180);
    var y2 = 50 * Math.sin((90-j2)*Math.PI/180);
    
    canvas.clear();
    canvas.attr({
        lineWidth: 3,
        lineCap: "round",
        lineJoin: "round",
        strokeStyle: "#B59554"
    }).begin().moveTo(50, 10)
    .lineTo(50+x1, 10+y1)
    .moveTo(50, 10)
    .lineTo(110, 10)
    .lineTo(110+x2, 10+y2)
    .stroke();
}


//192.168.1.133:10000/?req=query+$port&port=/MyCubInterface/cmd:i
function resolvesAdress(mycub_address, mycub_ports) {
    var request = "http://" + mycub_address + ":" + mycub_ports['/mycub'];
    $.getJSON( request, {req: 'query $port', port: '/MyCubInterface/cmd:i' } )
    .done(function( data ) {
        mycub_ports['/MyCubInterface/cmd:i'] = data["port_number"];
    });

    $.getJSON( request, {req: 'query $port', port: '/GazeControl/cmd:i' } )
    .done(function( data ) {
        mycub_ports['/GazeControl/cmd:i'] = data["port_number"];
        updateCamPose();
    });

    $.getJSON( request, {req: 'query $port', port: '/grabber' } )
    .done(function( data ) {
        mycub_ports['/grabber'] = data["port_number"];
        $("#scaled-frame").attr("src", "http://"+mycub_address+":"+ data["port_number"]+"/?action=stream");
    });
}


var bStopCam = false;
//?req=joint+(+$p1+$p2+)&p1=80&p2=105
function moveCamJoint(j1, j2, s) {
    if(bStopCam == true) return;
    bStopCam = true;
    var request = "http://" + mycub_address + ":" + mycub_ports['/GazeControl/cmd:i'];    
    $.getJSON( request, 
                { req:   'joint ($pos1 $pos2)', 
                  pos1: j1,
                  pos2: j2
                } )
    .done(function( data ) {
        bStopCam = false;
        //window.alert(data[0])//mycub_ports['/MyCubInterface/cmd:i'] = data["port_number"];
    });
}

var bStop = false;
function moveJoint(j, p, s) {
    if(bStop == true) return;
    bStop = true;
    var request = "http://" + mycub_address + ":" + mycub_ports['/MyCubInterface/cmd:i'];
    //var request = "http://192.168.1.158:10000";
    $.getJSON( request, 
                { req:   'gotoPose $joint $pose $speed', 
                  joint: j,
                  pose:  p,
                  speed: s
                } )
    .done(function( data ) {
        bStop = false;
        //window.alert(data[0])//mycub_ports['/MyCubInterface/cmd:i'] = data["port_number"];
    });
}


function updateAllStatus() {
    isConnected = false;
    var request = "http://" + mycub_address + ":" + mycub_ports['/MyCubInterface/cmd:i'];
    $.getJSON( request, 
                { req:   'getAll'} )
    .done(function( data ) {
            isConnected = true;
            //vector of j0, j1, j2, j3, dist, heading, volt
            $("#value_j1").val(data[0][0]);
            r = data[0][0]*Phoria.RADIANS - plate_front.angle;
            plate_front.angle = data[0][0]*Phoria.RADIANS;
            plate_front.rotateZ(r);

            $("#value_j2").val(data[0][1]);
            r = data[0][1]*Phoria.RADIANS - plate_right.angle;
            plate_right.angle = data[0][1]*Phoria.RADIANS;
            plate_right.rotateZ(-r);

            $("#value_j3").val(data[0][2]);
            r = data[0][2]*Phoria.RADIANS - plate_back.angle;
            plate_back.angle = data[0][2]*Phoria.RADIANS;
            plate_back.rotateZ(-r);

            $("#value_j4").val(data[0][3]);
            r = data[0][3]*Phoria.RADIANS - plate_left.angle;
            plate_left.angle = data[0][3]*Phoria.RADIANS;
            plate_left.rotateZ(r);

            drawFront(canvas_rear, data[0][0], data[0][2]);
            drawFront(canvas_front, data[0][1], data[0][3]);
            //$("#arrow").rotate(80);
            $("#arrow").attr("style","-webkit-transform:rotate("+data[0][5]+"deg); -moz-transform:rotate("+data[0][5]+"deg); -o-transform:rotate("+data[0][5]+"deg); -ms--transform:rotate("+data[0][5]+"deg);");
            /*
            r = (data[0][5]%180)*Phoria.RADIANS - plate_top.Zangle;
            plate_top.Zangle = (data[0][5]%180)*Phoria.RADIANS;
            plate_top.rotateY(r);
            //plate_front.rotateX(r);
            //plate_back.rotateX(-r);
            //plate_right.rotateX(-r);
            */
            setBatteryLevel(data[0][6]);
            setSonarLevel(data[0][4]);
    });
}


function updateCamPose() {
    var request = "http://" + mycub_address + ":" + mycub_ports['/GazeControl/cmd:i'];
    $.getJSON( request, 
                { req:   'get joints'} )
    .done(function( data ) {
            $("#value_h1").val(data[0]);
            $("#value_h2").val(data[1]);
    });
}

function setBatteryLevel(milivolt) {
    $('#battery_voltage').text(milivolt/1000 + " V");
    level = (milivolt-6000) / (7400-6000) * 250;
    if(level>250) {
        level = 250;
    }
    if(level<0) {
        level=0;
    }
    width = 250 - level;
    left = 111 + level;
    $("#battery_value").attr("style", "width:"+width+"px; left:"+left+"px;");
}

function setSonarLevel(dist) {
    $('#sonar_value').text(dist/10 + " cm");
    h = 165 - (dist*175/3000 - 5);
    if(h>165) { h = 165; }
    if(h<-10) { h = -10; }    
    $("#bullet").attr("style", "top:"+h+"px;");    
}

function updateConnectionStatus() {
    if(isConnected == true) {
        $("#connection_status").attr("style", "color:#447821;");
        $('#connection_status').text("Connected");
    }
    else {
        $("#connection_status").attr("style", "color:#784421;");
        $('#connection_status').text("Disconnected");
    }
}

