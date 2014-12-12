
var requestAnimFrame = window.requestAnimationFrame || window.webkitRequestAnimationFrame ||
                       window.mozRequestAnimationFrame || window.msRequestAnimationFrame || 
                       function(c) {window.setTimeout(c, 1000)};

// bind to window onload event
//window.addEventListener('load', onloadHandler, false);
var bitmaps = [];
var scene = {};
var plate_top = {};
var plate_front = {};
var plate_back = {};
var plate_right = {};
var plate_left = {};

function onloadHandler()
{
    // get the images loading
    var loader = new Phoria.Preloader();   
    bitmaps.push(new Image());
    loader.addImage(bitmaps[0], 'wood.png');
    loader.onLoadCallback(init);
}

function init()
{
   // get the canvas DOM element and the 2D drawing context
   var canvas = document.getElementById('canvas_3d');
   // create the scene and setup camera, perspective and viewport
   scene = new Phoria.Scene();
   scene.camera.position = {x:8.0, y:8.0, z:-15.0};
   scene.perspective.aspect = canvas.width / canvas.height;
   scene.viewport.width = canvas.width;
   scene.viewport.height = canvas.height;
   
   // create a canvas renderer
   var renderer = new Phoria.CanvasRenderer(canvas);
   
   // add a grid to help visualise camera position etc.
   var plane = Phoria.Util.generateTesselatedPlane(16,16,0,30);
   scene.graph.push(Phoria.Entity.create({
      points: plane.points,
      edges: plane.edges,
      polygons: plane.polygons,
      style: {
         shademode: "plain",
         drawmode: "wireframe",
         linewidth: 0.5,
         objectsortmode: "back"
      }
   }));

   //plate_bottom = createPart();

   plate_top = createPart();
   plate_top.translateY(2.1);

  
   plate_front = createSidePart();
   plate_front.translateX(2.05);
   plate_front.translateY(2.0);
   plate_front.rotateZ(-Math.PI/2);

   plate_back = createSidePart();   
   plate_back.translateX(-2.05);
   plate_back.translateY(2.0);
   plate_back.rotateZ(-Math.PI/2);
   

   plate_left = createSidePart();   
   plate_left.rotateY(-Math.PI/2);
   plate_left.rotateZ(-Math.PI/2);
   plate_left.translateX(-2.05);
   plate_left.translateY(2.0);

   plate_right = createSidePart();   
   plate_right.rotateY(-Math.PI/2);
   plate_right.rotateZ(-Math.PI/2);
   plate_right.translateX(-2.05);
   plate_right.translateY(-2.0);

   plate_top.Zangle = 0;
   plate_front.angle = 0;   
   plate_right.angle = 0;   
   plate_back.angle = 0;   
   plate_left.angle = 0;   

   // execute the model view 3D pipeline and render the scene
   scene.modelView();
   renderer.render(scene);


   // add a light
   scene.graph.push(Phoria.DistantLight.create({
      direction: {x:0, y:-0.5, z:1}
   }));
   
   
   // mouse rotation and position tracking
   var mouse = Phoria.View.addMouseEvents(canvas);
   
   // keep track of rotation
   var rot = {
      x: 0, y: 0, z: 0,
      velx: 0, vely: 0, velz: 0,
      nowx: 0, nowy: 0, nowz: 0,
      ratio: 0.1
   };

   
   var pause = false;
   
   var fnAnimate = function() {
      if (!pause)
      {
         /*
         // rotate local matrix of the cube
         rot.nowx += (rot.velx = (mouse.velocityV - rot.x - rot.nowx) * rot.ratio);
         rot.nowy += (rot.vely = (rot.y - rot.nowy) * rot.ratio);
         rot.nowz += (rot.velz = (mouse.velocityH - rot.z - rot.nowz) * rot.ratio);        
         //plate_front.rotateX(-rot.velx*Phoria.RADIANS).rotateY(-rot.vely*Phoria.RADIANS).rotateZ(-rot.velz*Phoria.RADIANS);
         plate_front.rotateZ(-rot.velz*Phoria.RADIANS);
         */
         // execute the model view 3D pipeline and render the scene
         scene.modelView();
         renderer.render(scene);
      }
      requestAnimFrame(fnAnimate);
   };
   

   /*
   // add GUI controls
   var gui = new dat.GUI();
   var f = gui.addFolder('Perspective');
   f.add(scene.perspective, "fov").min(5).max(175);
   f.add(scene.perspective, "near").min(1).max(100);
   f.add(scene.perspective, "far").min(1).max(1000);
   //f.open();
   f = gui.addFolder('Camera LookAt');
   f.add(scene.camera.lookat, "x").min(-100).max(100);
   f.add(scene.camera.lookat, "y").min(-100).max(100);
   f.add(scene.camera.lookat, "z").min(-100).max(100);
   f.open();
   f = gui.addFolder('Camera Position');
   f.add(scene.camera.position, "x").min(-100).max(100);
   f.add(scene.camera.position, "y").min(-100).max(100);
   f.add(scene.camera.position, "z").min(-100).max(100);
   f.open();
   f = gui.addFolder('Camera Up');
   f.add(scene.camera.up, "x").min(-10).max(10).step(0.1);
   f.add(scene.camera.up, "y").min(-10).max(10).step(0.1);
   f.add(scene.camera.up, "z").min(-10).max(10).step(0.1);
   f = gui.addFolder('Spin Local Axis (or use mouse)');
   f.add(rot, "x").min(-180).max(180);
   f.add(rot, "y").min(-180).max(180);
   f.add(rot, "z").min(-180).max(180);
   f.open();
   */ 
   
   // key binding
   document.addEventListener('keydown', function(e) {
      switch (e.keyCode)
      {
         case 27:
         {
            pause = !pause;
            break;
         }
      }
   }, false);
   
   // start animation
   requestAnimFrame(fnAnimate);
}

function generatePlate(scale)
{
  var s = scale || 1;
  return {
     points: [{x:-1*s,y:0.025*s,z:-1*s}, {x:1*s,y:0.025*s,z:-1*s}, {x:1*s,y:-0.025*s,z:-1*s}, {x:-1*s,y:-0.025*s,z:-1*s},
              {x:-1*s,y:0.025*s,z:1*s}, {x:1*s,y:0.025*s,z:1*s}, {x:1*s,y:-0.025*s,z:1*s}, {x:-1*s,y:-0.025*s,z:1*s}],
     edges: [{a:0,b:1}, {a:1,b:2}, {a:2,b:3}, {a:3,b:0}, {a:4,b:5}, {a:5,b:6}, {a:6,b:7}, {a:7,b:4}, {a:0,b:4}, {a:1,b:5}, {a:2,b:6}, {a:3,b:7}],
     polygons: [{vertices:[0,1,2,3]},{vertices:[1,5,6,2]},{vertices:[5,4,7,6]},{vertices:[4,0,3,7]},{vertices:[4,5,1,0]},{vertices:[3,2,6,7]}]
  };
}

function generateSidePlate(scale)
{
  var s = scale || 1;
  return {
     points: [{x:0*s,y:0.025*s,z:-1*s}, {x:2*s,y:0.025*s,z:-1*s}, {x:2*s,y:-0.025*s,z:-1*s}, {x:0*s,y:-0.025*s,z:-1*s},
              {x:0*s,y:0.025*s,z:1*s}, {x:2*s,y:0.025*s,z:1*s}, {x:2*s,y:-0.025*s,z:1*s}, {x:0*s,y:-0.025*s,z:1*s}],
     edges: [{a:0,b:1}, {a:1,b:2}, {a:2,b:3}, {a:3,b:0}, {a:4,b:5}, {a:5,b:6}, {a:6,b:7}, {a:7,b:4}, {a:0,b:4}, {a:1,b:5}, {a:2,b:6}, {a:3,b:7}],
     polygons: [{vertices:[0,1,2,3]},{vertices:[1,5,6,2]},{vertices:[5,4,7,6]},{vertices:[4,0,3,7]},{vertices:[4,5,1,0]},{vertices:[3,2,6,7]}]
  };
}


function createPart() {
   c = generatePlate(2.0);
   var plate = Phoria.Entity.create({
      points: c.points,
      edges: c.edges,
      polygons: c.polygons
   });
   for (var i=0; i<6; i++)
   {
      //cube.polygons[i].color = [42*i, 256-(42*i), (128+(42*i))%256];
      plate.textures.push(bitmaps[0]);
      plate.polygons[i].texture = i;
   }
   scene.graph.push(plate);
   
   Phoria.Entity.debug(plate, {      
      showAxis: true
      //showPosition: true
   });
    
   return plate;
}

function createSidePart() {
   c = generateSidePlate(2.0);
   var plate = Phoria.Entity.create({
      points: c.points,
      edges: c.edges,
      polygons: c.polygons
   });
   for (var i=0; i<6; i++)
   {
      //cube.polygons[i].color = [42*i, 256-(42*i), (128+(42*i))%256];
      plate.textures.push(bitmaps[0]);
      plate.polygons[i].texture = i;
   }
   scene.graph.push(plate);
   
  /*
   Phoria.Entity.debug(plate, {      
      showAxis: true
      //showPosition: true
   });
   */
   return plate;
}

