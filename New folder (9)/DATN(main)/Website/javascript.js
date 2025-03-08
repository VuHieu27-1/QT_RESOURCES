jQuery(document).ready(function(){
      var f1 = firebaseRef = firebase.database().ref().child("nhakinh/nhietdo");
		f1.on('value',function(datasnapshot){
			nhietdo2.innerText = datasnapshot.val();
		});
		var f2 = firebaseRef = firebase.database().ref().child("nhakinh/doam");
		f2.on('value',function(datasnapshot){
			doam2.innerText = datasnapshot.val();
		});
		var f3 = firebaseRef = firebase.database().ref().child("nhakinh/doamdat");
		f3.on('value',function(datasnapshot){
			doamdat2.innerText = datasnapshot.val();
		});
		var f4 = firebaseRef = firebase.database().ref().child("nhakinh/khongkhi");
		f4.on('value',function(datasnapshot){
			air2.innerText = datasnapshot.val();
		});
		var f9 = firebaseRef = firebase.database().ref().child("nhakinh/anhsang");
		f9.on('value',function(datasnapshot){
			anhsang2.innerText = datasnapshot.val();
		});
		$('#nut1').click(function() {
    	if($(this).is(':checked')) 
		{
       	firebase.database().ref().child("thietbi/switch1").set("1");
    	} else 
		{
       firebase.database().ref().child("thietbi/switch1").set("0");
    	}
    	firebase.database().ref().child("thietbi/allow1").set("1");
		});
		var f5 = firebaseRef = firebase.database().ref().child("thietbi/switch1");
		f5.on('value',function(datasnapshot){
			var nut1 = datasnapshot.val();	
			if (nut1 == "0" ){
			var isGenderMale = $('#nut1').prop('checked', 0);
			}
		else if (nut1 == "1" ){
			var isGenderMale = $('#nut1').prop('checked', 1);
			} 
		});
		$('#nut2').click(function() {
    	if($(this).is(':checked')) 
		{
       	firebase.database().ref().child("thietbi/switch2").set("1");
    	} else 
		{
       firebase.database().ref().child("thietbi/switch2").set("0");
    	}
    	firebase.database().ref().child("thietbi/allow2").set("1");
		});
		var f6 = firebaseRef = firebase.database().ref().child("thietbi/switch2");
		f6.on('value',function(datasnapshot){
			var nut2 = datasnapshot.val();	
			if (nut2 == "0" ){
			var isGenderMale = $('#nut2').prop('checked', 0);
			}
		else if (nut2 == "1" ){
			var isGenderMale = $('#nut2').prop('checked', 1);
			} 
		});
		$('#nut3').click(function() {
    	if($(this).is(':checked')) 
		{
       	firebase.database().ref().child("thietbi/switch3").set("1");
    	} else 
		{
       	firebase.database().ref().child("thietbi/switch3").set("0");
    	}
    	firebase.database().ref().child("thietbi/allow3").set("1");
		});
		var f7 = firebaseRef = firebase.database().ref().child("thietbi/switch3");
		f7.on('value',function(datasnapshot){
			var nut3 = datasnapshot.val();	
			if (nut3 == "0" ){
			var isGenderMale = $('#nut3').prop('checked', 0);
			}
		else if (nut3 == "1" ){
			var isGenderMale = $('#nut3').prop('checked', 1);
			} 
		});
		$('#nut4').click(function() {
    	if($(this).is(':checked')) 
		{
       	firebase.database().ref().child("thietbi/switch4").set("1");
      	 firebase.database().ref().child("thietbi/switch6").set("0");
    	} else 
		{
       firebase.database().ref().child("thietbi/switch4").set("0");
    	}
    	firebase.database().ref().child("thietbi/allow4").set("1");
		});
		var f8 = firebaseRef = firebase.database().ref().child("thietbi/switch4");
		f8.on('value',function(datasnapshot){
			var nut4 = datasnapshot.val();	
			if (nut4 == "0" ){
			var isGenderMale = $('#nut4').prop('checked', 0);
			}
		else if (nut4 == "1" ){
			var isGenderMale = $('#nut4').prop('checked', 1);
			} 
		});
		$('#nut5').click(function() {
    	if($(this).is(':checked')) 
		{
       	firebase.database().ref().child("thietbi/switch5").set("1");
    	} else 
		{
      	 firebase.database().ref().child("thietbi/switch5").set("0");
    	}
    	firebase.database().ref().child("thietbi/allow5").set("1");
		});
		var f10 = firebaseRef = firebase.database().ref().child("thietbi/switch5");
		f10.on('value',function(datasnapshot){
			var nut5 = datasnapshot.val();	
			if (nut5 == "0" ){
			var isGenderMale = $('#nut5').prop('checked', 0);
			}
		else if (nut5 == "1" ){
			var isGenderMale = $('#nut5').prop('checked', 1);
			} 
		});
		$('#nut6').click(function() {
    	if($(this).is(':checked')) 
		{
       	firebase.database().ref().child("thietbi/switch6").set("1");
      	 firebase.database().ref().child("thietbi/switch4").set("0");
    	} else 
		{
      	 firebase.database().ref().child("thietbi/switch6").set("0");
    	}
    	firebase.database().ref().child("thietbi/allow6").set("1");
		});
		var f15 = firebaseRef = firebase.database().ref().child("thietbi/switch6");
		f15.on('value',function(datasnapshot){
			var nut6 = datasnapshot.val();	
			if (nut6 == "0" ){
			var isGenderMale = $('#nut6').prop('checked', 0);
			}
		else if (nut6 == "1" ){
			var isGenderMale = $('#nut6').prop('checked', 1);
			} 
		});
		var myVar=setInterval(function(){Clock()},1000);
		function Clock() {
		a=new Date();
		w=Array("Chủ Nhật","Thứ hai","Thứ ba","Thứ tư","Thứ năm","Thứ sáu","Thứ bảy");
		var a=w[a.getDay()],
		w=new Date,
		d=w.getDate();
		m=w.getMonth()+1;
		y=w.getFullYear();
		h=w.getHours();
		mi=w.getMinutes();
		se=w.getSeconds();
		if(10>d){d="0"+d}
		if(10>m){m="0"+m}
		if(10>h){h="0"+h}
		if(10>mi){mi="0"+mi}
		if(10>se){se="0"+se}
		document.getElementById("date").innerHTML="Hôm nay: "+a+", "+d+" / "+m+" / "+y+" - "+h+":"+mi+":"+se+"";
		}
	});
