

/**
 *  some constants and global vars
 */
var mycub_address = "192.168.1.133";
var mycub_ports = new Object(); // or just {}
mycub_ports['/mycub'] = "10000";
mycub_ports['/MyCubInterface/cmd:i'] = "";
mycub_ports['/GazeControl/cmd:i'] = "";

var canvas_front;
var canvas_rear;
var canvas_top;

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
   var $pbar = $("#slider_j1").zinoSlider({
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

    canvas_top = zino.Canvas({
        target: document.getElementById("canvas_top"),
        width: 160,
        height: 120
    });

    /*
    $("#chart_compass").zinoChart({
        type: "rose",
        width: 160,
        height:160,
        radius: 50,
        legend: false,
        categories: [{
            "category": [
                {"label": 2011},
                {"label": 2013}
            ]
        }],
        series: [{
            "label": "NY Knicks",
            "color": "#3366CC",
            "data": [
                {"value": 9},
                {"value": 2}
            ]
        },{
            "label": "LA Clippers",
            "color": "#067A06",
            "data": [
                {"value": 0},
                {"value": 0}
            ]
        }]
    });
    */

    drawFront(canvas_front, 0, 0);
    drawFront(canvas_rear, 0, 0);

    resolvesAdress(mycub_address, mycub_ports);

    setInterval(function() {
        updateAllStatus();
        updateCamPose();
    }, 1000);

});

function drawFront(canvas, j1, j2) {
    var x1 = 50 * Math.cos((90+j1)*Math.PI/180);
    var y1 = 50 * Math.sin((90+j1)*Math.PI/180);
    var x2 = 50 * Math.cos((90-j2)*Math.PI/180);
    var y2 = 50 * Math.sin((90-j2)*Math.PI/180);
    
    $('#x').html(j1);
    $('#y').html(j2);

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
    var request = "http://" + mycub_address + ":" + mycub_ports['/MyCubInterface/cmd:i'];
    $.getJSON( request, 
                { req:   'getAll'} )
    .done(function( data ) {
            //vector of j0, j1, j2, j3, dist, heading, volt
            $("#value_j1").val(data[0][0]);
            $("#value_j2").val(data[0][1]);
            $("#value_j3").val(data[0][2]);
            $("#value_j4").val(data[0][3]);
            drawFront(canvas_rear, data[0][0], data[0][2]);
            drawFront(canvas_front, data[0][1], data[0][3]);
            //$("#arrow").rotate(80);
            $("#arrow").attr("style","-webkit-transform:rotate("+data[0][5]+"deg); -moz-transform:rotate("+data[0][5]+"deg); -o-transform:rotate("+data[0][5]+"deg); -ms--transform:rotate("+data[0][5]+"deg);");
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

