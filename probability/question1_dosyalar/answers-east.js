( function() {
    if(window.chitika_units === undefined) {
	window.chitika_units = [];
    }
    
    var query = document.title.replace(/ - Yahoo!.*$/, '');
    
    var unit = {
	'client' : 'yahoo_answers_east',
	'type' : 'mpu',
	'width' : 300,
	'height' : 250,
	'cid' : '',
	'sid' : 'default',
	'query' : query
    };
    
    var placement_id = window.chitika_units.length;
    
    window.chitika_units.push(unit);
    
    document.write('<div id="chitikaAdBlock-' + placement_id.toString() + '"></div>');
    
    // At this point, we have "default only" conditions.
    
    var isClosedQuestion = false, answerBox = false, targetBox = false;
    
    // Check if we're on an appropriate page
    try {
	targetBox = document.getElementById("yan-main");
	var top_divs = targetBox.getElementsByTagName('div');
	for(var i = 0; i < top_divs.length; ++i) {
	    if(top_divs[i].className && top_divs[i].className.indexOf('answer') !== -1 && top_divs[i].className.indexOf('best') !== -1) {
		isClosedQuestion = true;
		answerBox = top_divs[i];
		break;
	    }
	}
    } catch(e) {
    }
    
    // If we're in move conditions, then tell HQ to move it
    var beacon = document.getElementById("chitikaAdBlock-" + placement_id.toString());
    beacon.style.padding = "0px 20px 14px 12px";
    beacon.style.margin = "0 0 0 99px";
    if(beacon) {
	unit.client = 'yahoo_answers_east';
	unit.width = 498;
	unit.height = 190;
	
	if(isClosedQuestion) {
	    targetBox.insertBefore(beacon.parentNode.removeChild(beacon), answerBox);
	    unit.cid = 'north_closed';
	    unit.sid = 'north_closed';
	    
	    var south_beacon = beacon.cloneNode(true);
	    south_beacon.id = "chitikaAdBlock-" + placement_id.toString() + '-1';
	    
	    if(answerBox.nextSibling) {
		targetBox.insertBefore(south_beacon, answerBox.nextSibling);
	    } else {
		targetBox.appendChild(south_beacon);
	    }
	    unit.hasClones = 1;
	} else {
	    targetBox.appendChild(beacon);
	    unit.cid = 'north_open';
	    unit.sid = 'north_open';
	}
    }
    var s = document.createElement('script');
    s.type = 'text/javascript';
    s.src = 'http://scripts.chitika.net/eminimalls/amm-yahoo.js';
    
    try {
	document.getElementsByTagName('head')[0].appendChild(s);
    } catch(e) {
	document.write(s.outerHTML);
    }
}());
