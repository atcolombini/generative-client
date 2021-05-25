
//*****Variables******/
/********************/

// General
const shapesPerSide = 4;
const canvasSize = 600;

var currentSelectedPieceID;

//Drawing Shapes
  //ellipse
  var ellipseSize = 6;

  //triangle
    //points
    var triangleDimensions;
    var aX;
    var aY;
    var bX;
    var bY;
    var cX;
    var cY;

    //animation values
    var triDirectionChanger = 1;
    var triGoBack = true;
    var triScaleY = 1;

//General Animation 
  var animationTempo = 0; // this always is between 0,1
  var animationSpeed = 0; // increments tempo per frame. Controlled by Wheel
  const animationSpeedModificationStep = 0.01;
  const maxAnimationSpeed = 0.08;

  var animationLoop = 0;

  var timer = 0;
  var timerFreezed = false;

  const maxRotation = 90;

//Shapes Amount
  var totalShapesAmount = 1;

//Colors
  var noColor; //StartColor
  var colorR;
  var colorRG; //yellow
  var colorRB; //magenta
  var colorG;
  var colorGB; //cyan
  var colorB;
  var colorRGB;
  
  var colorSelected;

//RBG input
  var keyR;
  var keyG;
  var keyB;

//Properties of shapes. Will be used later
  var shapesProperties;

/*** GIZMOS ****/
//Selection Gizmo
var selectionGizmoSize;
var selectionGizmoMinSize;
var selectionGizmoMaxSize
var growingSelectionGizmo;

function setup() {
  
  createCanvas(canvasSize, canvasSize);
  angleMode(DEGREES);
  frameRate(60);
    
  //Colors & Palette
  //PICK A GOOD PALETTE
  noColor  = color('rgb(0,0,0)'); //StartColor
  colorR   = color('rgb(235, 52, 52)');
  colorRG  = color('rgb(235, 229, 52)'); //yellow
  colorRB  = color('rgb(235, 52, 140)'); //magenta
  colorG   = color('rgb(58, 235, 52)');
  colorGB  = color('rgb(52, 235, 229)'); //cyan
  colorB   = color('rgb(52, 55, 235)');
  colorRGB = color('rgb(0,0,0)'); //ADD HERE A COLOR THAT MODIFIES USING BEZIER
  
  colorSelected = color('rgb(255, 255, 255)');
  
  //set color inputs to false
  keyR = false;
  keyG = false;
  keyB = false;
  
  //triangle setup (needs canvas created)
  triangleDimensions = canvasSize/11; // if /8 triangles go to borders of their slots
  aX = triangleDimensions;
  aY = -triangleDimensions;
  bX = 0;
  bY = triangleDimensions;
  cX = -triangleDimensions;
  cY = -triangleDimensions;
  
  currentSelectedPieceID = -1;
  
  //Setup of properties of Shapes (the last one, so it could use every other thing)
  shapesProperties = {
  
    shapeTypes: [         // The array "shapeTypes" determine the shape of each slot. 
      "quad",             // It contains 16 items that can be:
      "quad",             //   - quad
      "quad",             //   - tri
      "quad",             //   - circle
                          //   - cross
      "quad",
      "quad",
      "quad",
      "quad",

      "quad",
      "quad",
      "quad",
      "quad",

      "quad",
      "quad",
      "quad",
      "quad"
    ],
    
    shapeColors: [          // The array "shapeColors" determine the outline color of each slot. 
      noColor,              // It contains 16 colors from the color Palette:
      noColor,
      noColor,            
      noColor,
      
      noColor,
      noColor,
      noColor,
      noColor,
      
      noColor,
      noColor,
      noColor,
      noColor,

      noColor,
      noColor,
      noColor,
      noColor
      
    ],
    
    isShapeSelected:  [
      false,
      false,
      false,
      false,
      
      false,
      false,
      false,
      false,
      
      false,
      false,
      false,
      false,
      
      false,
      false,
      false,
      false     
    ]
  };
  
  /*** GIZMOS ***/
  selectionGizmoMinSize = 20;
  selectionGizmoMaxSize = 30;
  selectionGizmoSize = 20;
  growingSelectionGizmo = true;
  
  Serial.begin(9600);
}

function draw() {  
  Serial.read();

  //inputs that require to be check every frame 
  
  // checks the inputs of the artifact every frame
  artifactInput();

  // used as keyboard method to switch shape
  // Calls "modifyShapeType"
  if (mouseIsPressed && currentSelectedPieceID>=0){
    if (mouseX <= canvasSize && mouseY <= canvasSize){
      
      if      (mouseY <= (canvasSize/4)*1){
        modifyShapeType(currentSelectedPieceID,"quad");
      }   
      
      else if (mouseY <=(canvasSize/4)*2) {
        modifyShapeType(currentSelectedPieceID,"tri");
      }
      
      else if (mouseY <=(canvasSize/4)*3) {
        modifyShapeType(currentSelectedPieceID,"circle");
      }
      
      else if (mouseY <=(canvasSize/4)*4) {
        modifyShapeType(currentSelectedPieceID,"cross");
      }
      
    }
  }


  
  background(51);
  switch(totalShapesAmount){
      
    case 1:
      drawSomething  (shapesProperties.shapeTypes[0], (canvasSize/2), (canvasSize/2), 0);
      break;
    
    case 2:
      drawSomething  (shapesProperties.shapeTypes[0], (canvasSize/6)*2,(canvasSize/2)*1, 0);
      drawSomething  (shapesProperties.shapeTypes[1], (canvasSize/6)*4,(canvasSize/2)*1, 1);
      break;

    case 4:
      drawSomething  (shapesProperties.shapeTypes[0], (canvasSize/6)*2.15,(canvasSize/6)*2.15, 0);
      drawSomething  (shapesProperties.shapeTypes[1], (canvasSize/6)*2.15,(canvasSize/6)*3.85, 1);
      drawSomething  (shapesProperties.shapeTypes[2], (canvasSize/6)*3.85,(canvasSize/6)*2.15, 2);
      drawSomething  (shapesProperties.shapeTypes[3], (canvasSize/6)*3.85,(canvasSize/6)*3.85, 3);
      break;

    case 8:
      drawSomething  (shapesProperties.shapeTypes[0], (canvasSize/8)*1,(canvasSize/8)*3, 0);
      drawSomething  (shapesProperties.shapeTypes[1], (canvasSize/8)*3,(canvasSize/8)*3, 1);
      drawSomething  (shapesProperties.shapeTypes[2], (canvasSize/8)*5,(canvasSize/8)*3, 2);
      drawSomething  (shapesProperties.shapeTypes[3], (canvasSize/8)*7,(canvasSize/8)*3, 3);

      drawSomething  (shapesProperties.shapeTypes[4], (canvasSize/8)*1,(canvasSize/8)*5, 4);
      drawSomething  (shapesProperties.shapeTypes[5], (canvasSize/8)*3,(canvasSize/8)*5, 5);
      drawSomething  (shapesProperties.shapeTypes[6], (canvasSize/8)*5,(canvasSize/8)*5, 6);
      drawSomething  (shapesProperties.shapeTypes[7], (canvasSize/8)*7,(canvasSize/8)*5, 7);
      break;
      
    case 16:
      drawSomething  (shapesProperties.shapeTypes[0],(canvasSize/8)*1,(canvasSize/8)*1, 0);
      drawSomething  (shapesProperties.shapeTypes[1],(canvasSize/8)*3,(canvasSize/8)*1, 1);
      drawSomething  (shapesProperties.shapeTypes[2],(canvasSize/8)*5,(canvasSize/8)*1, 2);        
      drawSomething  (shapesProperties.shapeTypes[3],(canvasSize/8)*7,(canvasSize/8)*1, 3);         

      drawSomething  (shapesProperties.shapeTypes[4],(canvasSize/8)*1,(canvasSize/8)*3, 4);        
      drawSomething  (shapesProperties.shapeTypes[5],(canvasSize/8)*3,(canvasSize/8)*3, 5);        
      drawSomething  (shapesProperties.shapeTypes[6],(canvasSize/8)*5,(canvasSize/8)*3, 6);        
      drawSomething  (shapesProperties.shapeTypes[7],(canvasSize/8)*7,(canvasSize/8)*3, 7);        

      drawSomething  (shapesProperties.shapeTypes[8],(canvasSize/8)*1,(canvasSize/8)*5,  8);        
      drawSomething  (shapesProperties.shapeTypes[9],(canvasSize/8)*3,(canvasSize/8)*5,  9);          
      drawSomething  (shapesProperties.shapeTypes[10],(canvasSize/8)*5,(canvasSize/8)*5, 10);        
      drawSomething  (shapesProperties.shapeTypes[11],(canvasSize/8)*7,(canvasSize/8)*5, 11);        

      drawSomething  (shapesProperties.shapeTypes[12],(canvasSize/8)*1,(canvasSize/8)*7, 12);        
      drawSomething  (shapesProperties.shapeTypes[13],(canvasSize/8)*3,(canvasSize/8)*7, 13);        
      drawSomething  (shapesProperties.shapeTypes[14],(canvasSize/8)*5,(canvasSize/8)*7, 14);        
      drawSomething  (shapesProperties.shapeTypes[15],(canvasSize/8)*7,(canvasSize/8)*7, 15);        
      break;
  }
  
  animate();
  

    


  //debugs
 {
 // print ("timeFreezed?"+timerFreezed);
 // print ("timer="+timer);  
 // print ("animationTempo="+animationTempo);
 // print ("animationSpeed="+animationSpeed);  
 // print ("deltaTime/1k="+deltaTime/1000); 
 // print ("Shape ID is "+ currentSelectedPieceID);
 }
}

/*** FUNCTIONS ***/
/*****************/

/*** General Actions ***/
function animate() {
    /*** Animate (animation & pause bucle) ***/
   {
  if (timerFreezed== true) {
    timer = timer + (deltaTime/1000);
    
    if (timer >= 0.5) {
      timerFreezed = false;
      timer = 0;
      animationTempo = animationTempo + animationSpeed;
    }
    
  }
 
  if (timerFreezed == false) {   

    if (animationTempo >= 1 || animationTempo<= -1) {    

      timerFreezed = true;          
      animationTempo = 0;
      animationLoop++;
    } 

    else {     
      animationTempo = animationTempo + animationSpeed;
    }
  }
 }
  
}

//Triggered Stuff - These are called by the inputs, serial or keyboard

function modifyAnimationSpeed (value){
  
  //ADD HERE A COOLDOWN TO IMPROVE FEEL

  if (value > 0){    
    if (animationSpeed >= maxAnimationSpeed-animationSpeedModificationStep) animationSpeed = maxAnimationSpeed;
    else if (animationSpeed == -animationSpeedModificationStep) animationSpeed = 0;
    else animationSpeed = animationSpeed + value;   
  }
    
  if (value < 0){    
    if (animationSpeed <= -(maxAnimationSpeed-animationSpeedModificationStep)) animationSpeed = -maxAnimationSpeed;
    else if (animationSpeed == animationSpeedModificationStep) animationSpeed = 0;
    else animationSpeed = animationSpeed + value;   
  }
  

}

function modifyTotalAmountOfShapes (value){
  
  switch (value) {      
    case 1:
      totalShapesAmount = 1;
      break;
      
    case 2:
      totalShapesAmount = 2;
      break;

    case 4:
      totalShapesAmount = 4;
      break;
            
    case 8:
      totalShapesAmount = 8;
      break;
      
    case 16:
      totalShapesAmount = 16;
      break;      
  }
  
  // ADD PARTICLE EFFECT TO SHOW EVERYTIME SIZES CHANGE
  
}

function modifyShapeColor (){
  
  if (keyR == false &&  keyG == false &&  keyB == false){
    
    shapesProperties.shapeColors[currentSelectedPieceID] = noColor; //StartColor
  
  }
  
  else if (keyR == true &&  keyG == false &&  keyB == false){

    shapesProperties.shapeColors[currentSelectedPieceID] = colorR;
  
  }
  
  else if (keyR == true &&  keyG == true &&  keyB == false) {
    
    shapesProperties.shapeColors[currentSelectedPieceID] = colorRG; //yellow
    
  }

  else if (keyR == true &&  keyG == false &&  keyB == true) {
  
    shapesProperties.shapeColors[currentSelectedPieceID] = colorRB; //magenta
  
  }
  
  else if (keyR == false &&  keyG == true &&  keyB == false){
    
    shapesProperties.shapeColors[currentSelectedPieceID] = colorG;
    
  }
  
  else if (keyR == false &&  keyG == true &&  keyB == true){
    shapesProperties.shapeColors[currentSelectedPieceID] = colorGB; //cyan
  }
  
  else if (keyR == false &&  keyG == false &&  keyB == true){
    shapesProperties.shapeColors[currentSelectedPieceID] = colorB;
  }
  
  else if (keyR == true  &&  keyG == true  && keyB == true){
    shapesProperties.shapeColors[currentSelectedPieceID] = colorRGB;
  }
  
}

function modifyShapeType (shapeID, shapeType){
    shapesProperties.shapeTypes[shapeID] = shapeType;
}

//Select/Unselect
function selectShape (shapeID){
  
  //SELECTION GIZMO OVERLAPS WITH A SELECTION INTERACTION :(
  
  if(shapesProperties.isShapeSelected[shapeID] == false) {    
    unselectAllShapes();
    shapesProperties.isShapeSelected[shapeID] = true;    
    currentSelectedPieceID = shapeID;

    ArtifactOutput.isShapeSelected = true;
  }
  
  else if (shapesProperties.isShapeSelected[shapeID] == true) {    
    unselectAllShapes(shapeID);
    currentSelectedPieceID = -1;
    
    ArtifactOutput.isShapeSelected = false;
  }    

  Serial.write();
}

function unselectAllShapes(){
  
  for (i=0; i<shapesProperties.isShapeSelected.length; i++) {      
    shapesProperties.isShapeSelected[i] = false;      
  }
  
}

/*** Draw Stuff ***/

function drawSomething(shapeTypeToDraw, positionX, positionY, shapeID){
  switch (shapeTypeToDraw){
    case "quad":
      drawQuad(positionX,positionY, shapeID);
      break;
      
    case "tri":
      drawTriangle(positionX,positionY, shapeID);
      break;

    case "circle":
      drawEllipse(positionX,positionY, shapeID);
      break;

    case "cross":
      drawCross (positionX,positionY, shapeID);
      break;
  }
}

function drawQuad(positionX,positionY, shapeID){
  push();
  modifyShapeColor();

  // check if selected
  if (shapesProperties.isShapeSelected[shapeID] == true){
    showSelectionGizmo(positionX, positionY);
  }
  
  //shape styles
  noFill();
  strokeWeight(1);
  stroke(shapesProperties.shapeColors[shapeID]);
  rectMode(CENTER);
  
  translate(positionX, positionY);
  
  rotate (animationTempo*maxRotation);
  
  //calculates the proper side size for the rectangle using Pitagora
  rect(0,0,(canvasSize/shapesPerSide)/sqrt(2),(canvasSize/shapesPerSide)/sqrt(2));
  
  pop();

}

function drawEllipse(positionX,positionY, shapeID){ //make so it can adapt to negative values
  push();  
  modifyShapeColor();

  if (shapesProperties.isShapeSelected[shapeID] == true){
    showSelectionGizmo(positionX, positionY);
  }
  
  //shape styles
  noFill();
  strokeWeight(1);
  stroke(shapesProperties.shapeColors[shapeID]);    
  translate(positionX, positionY);
  
  //Animates Ellipse (considers animationTempo + or -)
  {
  if (animationTempo>0){
    if (animationTempo > 0 && animationTempo < 0.5) {
      scale(1+animationTempo);
    }

    else if (animationTempo < 1 && animationTempo > 0.5) {
      scale(2-animationTempo);
    }

    else if (animationTempo == 0 || animationTempo ==1) {
      scale(1);
    }
  }
  
  if (animationTempo<0){
    
      if (animationTempo > -0.5) {
      scale(1-animationTempo);
    }

    else if (animationTempo > -1 && animationTempo < -0.5) {
      scale(2+animationTempo);
    }

    else if (animationTempo == 0 || animationTempo == -1) {
      scale(1);
    }  
  }
  }
  
  ellipse(0,0,(canvasSize/ellipseSize),(canvasSize/ellipseSize));
  
  pop();
}

let triangleCurrentScale;

function drawTriangle(positionX,positionY, shapeID){
  
  //let changeTriDirection;
  let posIncRelSpeed = 1;
  
  push();
  modifyShapeColor();

  if (shapesProperties.isShapeSelected[shapeID] == true){
    showSelectionGizmo(positionX, positionY);
  }
    
  noFill();
  strokeWeight(1);
  stroke(shapesProperties.shapeColors[shapeID]);
  
  translate(positionX, positionY);
    
  //Animates Triangle (considers animationTempo + or -)

  if(animationTempo > 0)
  {
    if(animationLoop % 2 == 0)
    {
      triangleCurrentScale = 1 - animationTempo * 2;
    }
    else
    {
      triangleCurrentScale = -1 + animationTempo * 2;
    }
  }
  else
  {
    if(animationLoop % 2 == 0)
    {
      triangleCurrentScale = 1 + animationTempo * 2;
    }
    else
    {
      triangleCurrentScale = -1 - animationTempo * 2;
    }
  }

  scale (1, triangleCurrentScale);
  triangle(aX,aY,bX,bY,cX,cY);
  
  pop();
    
}

function drawCross (positionX,positionY, shapeID){
  push();
  modifyShapeColor();

  if (shapesProperties.isShapeSelected[shapeID] == true){
    showSelectionGizmo(positionX, positionY);
  }

  //Shape styles
  noFill();
  strokeWeight(1);
  stroke(shapesProperties.shapeColors[shapeID]);  
  rectMode(CENTER);
  
  translate(positionX, positionY);
  rotate (45);
  rotate (animationTempo*maxRotation);
  
  //lines to draw figure
  {
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4
  );
  
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2
  );
  
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-4,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4
  );
  line(
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*-2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4, 
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*2,
    (((canvasSize/shapesPerSide)/sqrt(2))/8)*4
  );
  }
   
  pop();

}

/*** GIZMOS ***/
function showSelectionGizmo(xPos, yPos){  
  noStroke();

  fill(59, 59, 59);
  circle(xPos,yPos,selectionGizmoSize*1.3); 
  
  fill(242, 242, 242);
  circle(xPos,yPos,selectionGizmoSize/2);  
  
  //animation of gizmo (independent to Tempo)
  if      (growingSelectionGizmo == true) selectionGizmoSize = selectionGizmoSize +0.15;
  else if (growingSelectionGizmo == false)selectionGizmoSize = selectionGizmoSize -0.15;
  
  if (selectionGizmoSize == selectionGizmoMinSize) {
    growingSelectionGizmo = true;  
  }
  
  else if (selectionGizmoSize >= selectionGizmoMaxSize){
    growingSelectionGizmo = false;
  }
}

/*** INPUTS ***/

/*** Artifact Inputs ***/

function artifactInput()
{
  if(Artifact.length != 0)
  {
    artifactRGBInput();
    
    artifactEncoderInput();

    artifactDistanceInput();

    artifactSoundInput();

    artifactButtonMatrixInput();

    artifactRBGOutput();
  }
}

//Calls "modifyAnimationSpeed" 
function artifactEncoderInput()
{
  modifyAnimationSpeed(Artifact.encoder);
}

let previousKey;

//Calls "selectShape"
/*
  K1   k5   k9    k13
  k2   k6   k10   k14
  k3   k7   k11   k15
  k4   k8   k12   k16
*/
function artifactButtonMatrixInput()
{
  // Do not execute if the keypad has not been pressed
  let currentKey = Artifact.keypad;
  if(currentKey != 0 && !isNaN(currentKey) && currentKey != previousKey)
  {
    //console.log(currentKey);

    switch(totalShapesAmount)
    {
      case 1:
          selectShape(0);
          break;
      case 2:
        if(currentKey < Key.K9)
        {
          selectShape(0);
        }
        else
        {
          selectShape(1);
        }
        break
      case 4:
        if(currentKey == Key.K1 || currentKey == Key.K2 || currentKey == Key.K5 || currentKey == Key.K6)
        {
          selectShape(0);
        }
        else if(currentKey == Key.K3 || currentKey == Key.K4 || currentKey == Key.K7 || currentKey == Key.K8)
        {
          selectShape(1);
        }
        else if(currentKey == Key.K9 || currentKey == Key.K10 || currentKey == Key.K13 || currentKey == Key.K14)
        {
          selectShape(2);
        }
        else if(currentKey == Key.K11 || currentKey == Key.K12 || currentKey == Key.K15 || currentKey == Key.K16)
        {
          selectShape(3);
        }
        break;
      case 8:
        if(currentKey == Key.K1 || currentKey == Key.K2)
        {
          selectShape(0);
        }
        else if (currentKey == Key.K5 || currentKey == Key.K6)
        {
          selectShape(1);
        }
        else if(currentKey == Key.K9 || currentKey == Key.K10 )
        {
          selectShape(2);
        }
        else if ( currentKey == Key.K13 || currentKey == Key.K14)
        {
          selectShape(3);
        }
        else if(currentKey == Key.K3 || currentKey == Key.K4 )
        {
          selectShape(4);
        }
        else if ( currentKey == Key.K7 || currentKey == Key.K8)
        {
          selectShape(5);
        }
        else if(currentKey == Key.K11 || currentKey == Key.K12 )
        {
          selectShape(6);
        }
        else if ( currentKey == Key.K15 || currentKey == Key.K16)
        {
          selectShape(7);
        }
        break;
      case 16:
        if(currentKey == Key.K1) selectShape(0);
        else if (currentKey == Key.K5) selectShape(1);
        else if(currentKey == Key.K9)  selectShape(2);
        else if (currentKey == Key.K13) selectShape(3);
        else if (currentKey == Key.K2) selectShape(4);
        else if (currentKey == Key.K6) selectShape(5);
        else if (currentKey == Key.K10) selectShape(6);
        else if (currentKey == Key.K14) selectShape(7);
        else if(currentKey == Key.K3) selectShape(8);
        else if (currentKey == Key.K7) selectShape(9);
        else if(currentKey == Key.K11) selectShape(10);
        else if (currentKey == Key.K15) selectShape(11);
        else if (currentKey == Key.K4) selectShape(12);
        else if (currentKey == Key.K8) selectShape(13);
        else if (currentKey == Key.K12) selectShape(14);
        else if (currentKey == Key.K16) selectShape(15);
        break;
    }
  }

  previousKey = Artifact.keypad;
}

// Calls "modifyTotalAmountOfShapes"
function artifactDistanceInput()
{
  // The maximum distance is 20cm, the Artifact
  // will give us data only when active.

  if(Artifact.distanceActive)
  {
    let d = Artifact.distance;

    if(d <= 4)
    {
      modifyTotalAmountOfShapes(1);
    }
    else if (d <= 8)
    {
      modifyTotalAmountOfShapes(2);
    }
    else if (d <= 12)
    {
      modifyTotalAmountOfShapes(4);
    }
    else if (d <= 16)
    {
      modifyTotalAmountOfShapes(8);
    }
    else if (d <= 20)
    {
      modifyTotalAmountOfShapes(16);
    }
  }
}

let soundPeriod = 500;
let soundCount = 0;
let lastSoundRead = 0;
let soundThreshold = 450;

//
// Calls "modifyShapeType"
function artifactSoundInput()
{  
  if(Artifact.sound != undefined && Artifact.sound != 0
    && Artifact.sound < soundThreshold
    && millis() - lastSoundRead >= soundPeriod)
  {
    let shape;

    soundCount++;
    switch(soundCount % 4)
    {
      case 0:
      {
        shape = "quad";
        break;
      }
      case 1:
      {
        shape = "tri";
        break;
      }
      case 2:
      {
        shape = "circle";
        break;
      }
      case 3:
      {
        shape = "cross";
        break;
      }
    }

    modifyShapeType(currentSelectedPieceID, shape)

    lastSoundRead = millis();
  }
}

// Only set value if the switch is ON
function artifactRGBInput()
{
  keyR = Artifact.red;
  keyG = Artifact.green;
  keyB = Artifact.blue;
}

let previousKeyR = false;
let previousKeyG = false;
let previousKeyB = false;

function artifactRBGOutput()
{
  // If keys have not changed, exit
  if(keyR == previousKeyR &&
     keyG == previousKeyG &&
     keyB == previousKeyB)
  {
      return;
  }

  if (keyR == false &&  keyG == false &&  keyB == false)
  { 
    ArtifactOutput.color = Color.NONE;
  }
  else if (keyR == true &&  keyG == false &&  keyB == false)
  {
    ArtifactOutput.color = Color.RED;
  }
  else if (keyR == true &&  keyG == true &&  keyB == false)
  {
    ArtifactOutput.color = Color.YELLOW;
  }
  else if (keyR == true &&  keyG == false &&  keyB == true)
  {
    ArtifactOutput.color = Color.MAGENTA;
  }
  else if (keyR == false &&  keyG == true &&  keyB == false)
  {
    ArtifactOutput.color = Color.GREEN;
  }
  else if (keyR == false &&  keyG == true &&  keyB == true)
  {
    ArtifactOutput.color = Color.CYAN;
  }
  else if (keyR == false &&  keyG == false &&  keyB == true)
  {
    ArtifactOutput.color = Color.BLUE;
  }
  else if (keyR == true  &&  keyG == true  && keyB == true)
  {
    ArtifactOutput.color = Color.WHITE;
  }
  else if(keyR == false && keyG == false && keyB == false)
  {
    ArtifactOutput.color = Color.NONE;
  }

  Serial.write();

  previousKeyR = keyR;
  previousKeyG = keyG;
  previousKeyB = keyB;
}

/*** Desktop Controls ***/

function mouseWheel(event){ 
  //Substitutes artifactWheelInput
  //calls "modifyAnimationSpeed"
  
  if (event.delta == 100) modifyAnimationSpeed(+animationSpeedModificationStep);
    
    
  else if (event.delta == -100) modifyAnimationSpeed(-animationSpeedModificationStep);
   
  // print ("animationSpeed="+animationSpeed);
}

function keyPressed() {
  
// Calls "modifyTotalAmountOfShapes"
// Substitutes artifactDistanceInput
  {
  if (keyCode == 49) { //Keyboard 1 = 1 shape
    modifyTotalAmountOfShapes(1);
  }
  
  else if (keyCode == 50) { //Keyboard 2 = 2 shapes
    modifyTotalAmountOfShapes(2);

  }
    
  else if (keyCode == 51) { //Keyboard 3 = 4 shapes
    modifyTotalAmountOfShapes(4);

  }
    
  else if (keyCode == 52) { //Keyboard 4 = 8 shapes
    modifyTotalAmountOfShapes(8);

  }
    
  else if (keyCode == 53) { //Keyboard 5 = 16 shapes
    modifyTotalAmountOfShapes(16);
  }
  }
  
// Calls "selectShape"
// Substitutes artifactFourButtonsInput
  {
  /*
  Keyboard        ShapeID             KeyCodes
  
  q  w  e  r      0   1   2   3       81   87   69   82
  t  y  u  i  ->  4   5   6   7   ->  84   89   85   73
  a  s  d  f      8   9   10  11      65   83   68   70
  g  h  j  k      12  13  14  15      71   72   74   75
  
  */
  
  switch(totalShapesAmount){      

    case 1:
      {
      if(
      keyCode == 81 || 
      keyCode == 87 || 
      keyCode == 69 ||
      keyCode == 82 ||
      keyCode == 84 ||
      keyCode == 89 ||
      keyCode == 85 ||
      keyCode == 73 ||
      keyCode == 65 ||
      keyCode == 83 ||
      keyCode == 68 ||
      keyCode == 70 ||
      keyCode == 71 ||
      keyCode == 72 ||
      keyCode == 74 ||
      keyCode == 75         
      )
      {
        selectShape(0);
      }
      break;
      } 
    
    case 2:
      {
      if(
      keyCode == 81 || 
      keyCode == 87 ||
      keyCode == 84 ||
      keyCode == 89 ||
      keyCode == 65 ||
      keyCode == 83 ||
      keyCode == 71 ||
      keyCode == 72 
      )
      {
        selectShape(0);        
      }
      
      else if (        
      keyCode == 69 ||
      keyCode == 82 ||     
      keyCode == 85 ||
      keyCode == 73 ||
      keyCode == 68 ||
      keyCode == 70 || 
      keyCode == 74 ||
      keyCode == 75         
      )        
      {
        selectShape(1);
      }
      break;
      }

    case 4:
      {
      if(
      keyCode == 81 || 
      keyCode == 87 ||
      keyCode == 84 ||
      keyCode == 89
      )
      {
        selectShape(0);        
      }
        
      else if (  
      keyCode == 65 ||
      keyCode == 83 ||
      keyCode == 71 ||
      keyCode == 72 
      )
      {
        selectShape(1);        
      }
                
      else if (  
      keyCode == 69 ||
      keyCode == 82 ||
      keyCode == 85 ||
      keyCode == 73 
      )
      {
        selectShape(2);        
      }
                
      else if (  
      keyCode == 68 ||
      keyCode == 70 ||
      keyCode == 74 ||
      keyCode == 75 
      )
      {
        selectShape(3);        
      }
      
      break;
      }
      
    case 8:
      {
      if(
      keyCode == 81 || 
      keyCode == 84
      )
      {
        selectShape(0);        
      }
      
      else if(
      keyCode == 87 ||
      keyCode == 89
      )
      {
        selectShape(1);        
      }
      
      else if (        
      keyCode == 69 ||
      keyCode == 85
      )
      {
        selectShape(2);        
      }  
      
      else if(
      keyCode == 82 ||     
      keyCode == 73
      )
      {
        selectShape(3);        
      }
      
      else if (
      keyCode == 65 ||
      keyCode == 71
      )
      {
        selectShape(4);
      }
      
      else if (
      keyCode == 83 || 
      keyCode == 72         
      )        
      {
        selectShape(5);
      }
      
      else if (
      keyCode == 68 || 
      keyCode == 74         
      )        
      {
        selectShape(6);
      }
      
      else if (
      keyCode == 70 || 
      keyCode == 75         
      )        
      {
        selectShape(7);
      }
      break;
      }
      
    case 16:     
      {
      if     (keyCode == 81)  selectShape(0); 
      else if(keyCode == 87)  selectShape(1);
      else if(keyCode == 69)  selectShape(2);
      else if(keyCode == 82)  selectShape(3);
      else if(keyCode == 84)  selectShape(4);
      else if(keyCode == 89)  selectShape(5);
      else if(keyCode == 85)  selectShape(6);
      else if(keyCode == 73)  selectShape(7);
      else if(keyCode == 65)  selectShape(8);
      else if(keyCode == 83)  selectShape(9);
      else if(keyCode == 68)  selectShape(10);
      else if(keyCode == 70)  selectShape(11);
      else if(keyCode == 71)  selectShape(12);
      else if(keyCode == 72)  selectShape(13);
      else if(keyCode == 74)  selectShape(14);
      else if(keyCode == 75)  selectShape(15);

      break;
      }
  }
    
  }
  
//Calls "modifyShapeColor"
// Substitutes artifactRBGInput
  {  
  /*
  Keyboard     ShapeID         KeyCodes
  R  G  B  ->  z   x   c   ->  90   88   67
  */
    
    if (keyCode == 90) {
      
      if (keyR == false)     keyR = true;
      else if (keyR == true) keyR = false;
    }

    else if (keyCode == 88) {

      if (keyG == false)     keyG = true; 
      else if (keyG == true) keyG = false; 
    }

    else if (keyCode == 67) {

      if (keyB == false) keyB = true;
      else if (keyB == true) keyB = false;
    }
  
  //print("Red is "+ keyR + " // Green is "+ keyG + " // Blue is "+ keyB);
  }

  /***************/
  return false; // prevent any default behaviour
}

