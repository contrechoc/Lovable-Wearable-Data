//Gets the browser specific XmlHttpRequest Object
function getXmlHttpRequestObject() {
 if (window.XMLHttpRequest) {
    return new XMLHttpRequest(); //Mozilla, Safari ...
 } else if (window.ActiveXObject) {
    return new ActiveXObject("Microsoft.XMLHTTP"); //IE
 } else {
    //Display our error message
    alert("Your browser doesn't support the XmlHttpRequest object.");
 }
}

//Our XmlHttpRequest object
var receiveReq = getXmlHttpRequestObject();

//Initiate the AJAX request
function makeRequest(url, param) {
//If our readystate is either not started or finished, initiate a new request
 if (receiveReq.readyState == 4 || receiveReq.readyState == 0) {
   //Set up the connection to captcha_test.html. True sets the request to asyncronous(default) 
   receiveReq.open("POST", url, true);
   //Set the function that will be called when the XmlHttpRequest objects state changes
   receiveReq.onreadystatechange = updatePage; 

   //Add HTTP headers to the request
   receiveReq.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
   receiveReq.setRequestHeader("Content-length", param.length);
   receiveReq.setRequestHeader("Connection", "close");
 
   //Make the request
   receiveReq.send(param);
 }   
}

//Called every time our XmlHttpRequest objects state changes
function updatePage() {
 //Check if our response is ready
 if (receiveReq.readyState == 4) {
   //Set the content of the DIV element with the response text
   document.getElementById('result').innerHTML = receiveReq.responseText;
  //document.getElementById('name1').innerHTML = receiveReq.responseText;
   //Get a reference to CAPTCHA image
   img = document.getElementById('imgCaptcha'); 
   //Change the image
   img.src = 'create_image.php?' + Math.random();
 }
}

function getList(theForm) {
 //Set the URL
var rad_val = "habitable";

for (var i=0; i < theForm.radio.length; i++)
   {
   if (theForm.radio[i].checked)
      {
      rad_val = theForm.radio[i].value;
      
      //document.getElementById('prop').innerHTML = rad_val;
      }
   }
 
 document.location.href = ("http://www.contrechoc.com/exovest/adopt_exoplanet.php"+"?indication="+rad_val+"&name1="+document.getElementById('name1').value.replace(/\s+/g, ' ')+"&name2="+document.getElementById('name2').value.replace(/\s+/g, ' ')).replace(/\s+/g, ' ');
}

//Called every time when form is perfomed
function getParam(theForm) {

 //Set the URL
 var url = 'captcha.php';
 //Set up the parameters of our AJAX call
 var postStr = theForm.txtCaptcha.name + "=" + encodeURIComponent( theForm.txtCaptcha.value );

 var first_name = document.getElementById('name1').value.replace(/\s+/g, ' ');
 var last_Name = document.getElementById('name2').value.replace(/\s+/g, ' ');
 var ID = document.getElementById('exoPlanet').value.replace(/\s+/g, ' ');
 
 if (  ( first_name == "" ) ||  ( last_Name == "" ) || ( ID == "" ) ){
 
 	 alert ( "some fields are empty" );

}
else
{
 makeRequest(url, postStr);
 
 var alertSTr = "You: " + first_name + " " + last_Name + " are adopting exoplanet number " + ID;
 var adoptSTr = first_name + " " + last_Name + " " + ID;
 adoptSTr = adoptSTr.replace(/\s+/g, ' ');
 var adoptUrl = "saveAdopter.php?q="+ adoptSTr + "&t=" + Math.random();
 makeRequest( adoptUrl, "" );
 }
 
}
 
