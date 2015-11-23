/*
	QUESTO OGGETTO ARRICCHISCE LA MAPPA 2d DI GOOGLE DI UN MENU CONTESTUALE ASSOCIATO AL CLICK DESTRO DEL MOUSE PER L' INSERZIONE DI MARKER 
	DRAGGABILI SULLA MAPPA , QUESTO MARKER SONO COLLEGATI IN ORDINE DI INSERIMENTO DA UNA PËOLYLLINE CHE RIPORTA LA MISURAZIONE DELLA DISTANZA 
	TYRA UN MARKER E L' ALTRO IN METRI O KILOMETRI
*/

function RulerMap( map ){
	
	// mappa di google 
	this.map = null ;
	
	// lista di marker nella mappa 
	this.marker_list = [] ;
	
	// lista di polylline che collegano un marker all' altro 
	this.polyline_list = [] ;
	
	// lista delle etichette dei metri 
	this.label_list = [] ;
	
	// indico se rendere le etcihette in metri o kilometri , di default sono i kilometri
	this.is_label_in_meter = true ;
	
	// menu contestuale del mouse 
	this.contextMenu = null ;
	
	// controllo i valori in input  
	if (( typeof map === 'undefined' ) || ( map === null )) {
		// non posso fare altro 
		return ;
	}

	// salvo la mappa 
	this.map = map ;
	
	// creo il menu contestuale per gestire i marker 
	
	// creo le opzioni del menu , associando lo style 
	var contextMenuOptions={};
	contextMenuOptions.classNames={menu:'context_menu',menuSeparator:'context_menu_separator'};

	//	creo un array del menu contestuale 
	var menuItems=[];
	menuItems.push({className:'context_menu_item', eventName:'add_distance_marker', label:'Distanza fino a qui'});
	menuItems.push({className:'context_menu_item', eventName:'delete_last_distance_marker', label:'Cancella ultimo punto'});
	menuItems.push({className:'context_menu_item', eventName:'delete_all_distance_marker', label:'Cancella misurazione'});
	//	se indico un item senza proprieta allora Ë un separatore 
	menuItems.push({});		
	menuItems.push({className:'context_menu_item', eventName:'set_label_in_meter', label:'misurazione in metri'});
	menuItems.push({className:'context_menu_item', eventName:'set_label_in_kilometer', label:'misurazione in Kilometri'});

	contextMenuOptions.menuItems=menuItems;

	//	creo il menu 
	this.contextMenu = new ContextMenu(map, contextMenuOptions);

	// indico di visualizzare il menu con il click destro del mouse 
	with ({ obj: this }) {
		google.maps.event.addListener(map, 'rightclick', function(mouseEvent){
			obj.contextMenu.show(mouseEvent.latLng);
		});
	}

	//	listenere per gli eventi del menu una volta selezionati 
	with ({ obj: this }) {
		google.maps.event.addListener(obj.contextMenu, 'menu_item_selected', function(latLng, eventName){
			// klatLng Ë la posizione del menu contestuale , 
			// eventname Ë l' identificativo inserito nelle opzioni del menu contestuale 
			
			// seleziono l' azione da eseguire 
			switch(eventName){
				case 'add_distance_marker':
					obj.addDistanceMarker( latLng ) ;
					break;
				case 'delete_last_distance_marker' :
					obj.deleteLastDistanceMarker() ;
					break ;
				case 'delete_all_distance_marker':
					obj.deleteAllDistanceMarker() ;
					break;
				case 'set_label_in_meter' :
					obj.setLabelInMeter( true ) ;
					break ;
				case 'set_label_in_kilometer':
					obj.setLabelInMeter( false ) ;
					break;				
			}
		});	
	}

} ;

	// questa funzione segna se riportare le etichette in metri o kilometri 
RulerMap.prototype.setLabelInMeter = function( bool ){
	// controllo se sono gia in metri o kilometri 
	if ( this.is_label_in_meter == bool ) return ;
	// salvo le nuove impostazioni 
	this.is_label_in_meter = bool ;
	// devo ricalcolare tutte le etichette 
	for ( var i = 0 ; i < this.label_list.length ; i++ ){
		// prelvo i punti collegati all' etichetta corrente e trovo la loro posizione 
		var prev_latlng = this.marker_list[i].getPosition() ;
		var post_latlng = this.marker_list[i+1].getPosition() ;
		// trovo la loro distanza 
		var meter = google.maps.geometry.spherical.computeDistanceBetween(prev_latlng,post_latlng) ;
		// trovo la distanza in metri o kilometri 
		var str = this.getDistanceLabel( meter ) ;
		// trovo l' etichetta in cui piazare la distanza 
		var label = this.label_list[i] ;
		// inserisco la distanza 
		label.setValues({labelContent:str }) ;
	}
}	
	
// data la distanza in metri , questa funzione restituisce l' etichetta da visualizzare
RulerMap.prototype.getDistanceLabel = function( meter ){
	// controllo se devo riportare i metri o i kilometri 
	if ( this.is_label_in_meter == true ) {
		// tronco i metri 
		var meter_str = parseInt( meter ) ;
		// aggiungo l' etichetta dei metri 
		meter_str = meter_str + " m" ;
		return meter_str ;			
	} else {
		// mostro solo le ultime due cifre dopo la virgola , quindi per adesso divido per 10 e tronco 
		meter /= 10 ;
		meter = parseInt( meter ) ;
		// adesso ho i decametri , divido per 100 e ho i kilometri
		meter /= 100 ;
		// trovo la stringa 
		kilometer_str = meter + " Km" ;
		return kilometer_str ;
	}
} ;	
	
	// questa funzione restituisce il pnuto intermdio tra due latlng di google 
RulerMap.prototype.getMiddleLatLng = function( latlng_1 , latlng_2 ){
	// prlevo lat e lon intermedi
	var lat = ( latlng_1.lat() + latlng_2.lat() ) / 2 ;
	var lon = ( latlng_1.lng() + latlng_2.lng() ) / 2 ;
	// costruisco il punto 
	var latlng = new google.maps.LatLng(lat , lon ) ;
	return latlng ;
}
	
	// funzione che spostan un marker 
RulerMap.prototype.dragMarker = function( marker ){
	// controllo se ci sono polylline da spostare
	if ( this.polyline_list.length == 0 ) return ;
	// prlevo la libreria da utilizzare di google ( questo per aumentare la leggibilita del codice ) 
	var spherical = google.maps.geometry.spherical;
	// ci sono pollyline da spostare , prendo l' identificativo e posizione del marker 
	var marker_index = marker.get("id") ;
	var latlng = marker.getPosition() ;
	// sposto la polylline del punto precedente se presente 
	if ( marker_index > 0 ){
		// esiste la polylyline precedente , prendo il pnuto precedente 
		var prev_marker = this.marker_list[marker_index - 1] ;
		// prlevo la sua posizione 
		var prev_latlng = prev_marker.getPosition() ;
		// trovo la polyllyine collegata 
		var prev_poly = this.polyline_list[marker_index - 1] ;
		// modifico la posizione della polyline 
		prev_poly.setPath([ prev_latlng , latlng ]);
		// trovo la distanza tra i due punti in metri 
		var prev_meter = spherical.computeDistanceBetween(prev_latlng,latlng) ;
		// trovo la label nella polylyline 
		var prev_label = this.label_list[marker_index - 1] ;
		// sposto la label al nuovo punto intermedio 
		var middle_latlng = this.getMiddleLatLng( prev_latlng , latlng ) ;
		prev_label.setPosition(middle_latlng) ;
		// reimposto l' etichetta 
		var meter_label = this.getDistanceLabel( prev_meter ) ;
		prev_label.setValues({labelContent:meter_label }) ;
	}
	// controllo se ho una posizione successiva , quindi se non sono l' ultimo marker inserito 
	if ( marker_index < ( this.marker_list.length - 1 )){
		// esiste la polylyline successiva , prendo il pnuto successivo 
		var post_marker = this.marker_list[marker_index + 1] ;
		// prlevo la sua posizione 
		var post_latlng = post_marker.getPosition() ;	
		// trovo la polyllyine collegata 
		var post_poly = this.polyline_list[marker_index] ;
		// modifico la posizione della polyline 
		post_poly.setPath([ latlng , post_latlng ]);
		// trovo la distanza tra i due punti in metri 
		var post_meter = spherical.computeDistanceBetween(latlng,post_latlng) ;	
		// trovo la label nella polylyline 
		var post_label = this.label_list[marker_index] ;
		// sposto la label al nuovo punto intermedio 
		var middle_latlng = this.getMiddleLatLng(latlng,post_latlng) ;
		post_label.setPosition(middle_latlng) ;
		// reimposto l' etichetta 
		var meter_label = this.getDistanceLabel( post_meter ) ;
		post_label.setValues({labelContent:meter_label }) ;
	}
	
} 
	
	// questa funzione aggiunge un marker alla mappa 
RulerMap.prototype.addDistanceMarker = function(latLng){
	// trovo lat e lon del punto come strimnga 
	var lat_lng_str = "lat : " + latLng.lat() + "\nlon : " + latLng.lng() ;
	// costruisco un marker che puo essere spostato 
	var marker = new google.maps.Marker({
		position: latLng,
		draggable: true,
		map: this.map,
		title: lat_lng_str
	});
	// trovo l' indice del marker nella lista 
	var id_marker = this.marker_list.length ;
	// inserisco l' identificativo 
	marker.setValues({id: id_marker });	

	// controllo se devo inserire una polylline da questo marker a quello precedente 
	if ( id_marker > 0 ){
		// trovo l' altro punto a cui si deve collegare la polylline 
		// prlevo il marker precednte 
		var prev_marker = this.marker_list[id_marker-1] ;
		// prlevo la latitudine e longitudine 
		var prev_latLng = prev_marker.getPosition() ;
		// creo la lista 
		var point_list = [] ;
		point_list.push( prev_latLng ) ;
		point_list.push( latLng ) ;
		// aggiungo la polylline 
		var polyline = new google.maps.Polyline({
			path: point_list,
			geodesic: true,
			strokeColor: '#000000',
			strokeOpacity: 1.0,
			strokeWeight: 2 , 
			map : this.map
		  });
		// inserisco la polylline nella lista 
		this.polyline_list.push( polyline) ;
		// ho anche una etichetta , la inserisco in mezzo alla polylyline 
		var middle_latlng = this.getMiddleLatLng( prev_latLng , latLng ) ;
		// trovo la distanza tr i due punti 
		var spherical = google.maps.geometry.spherical ;
		var meter = spherical.computeDistanceBetween(prev_latLng,latLng) ;	
		// trovo letcihetta
		var meter_str = this.getDistanceLabel( meter ) ;
		// inserisco l' etichetta nel nuovo punto 
		var label = new MarkerWithLabel({
			position: middle_latlng,
			draggable: false,
			map: this.map,
			icon: " " , // nascondo l' icona 
			labelContent: meter_str ,
			labelAnchor: new google.maps.Point(0, 0),
			labelClass: "labels" // the CSS class for the label,
		});			
		// inserisco la mappa in lista 
		this.label_list.push( label) ;

	} ;
	
	with ({ obj: this }) {
		// inserisco un evento per indicarelo spostamento 
		google.maps.event.addListener(marker, 'drag', function() { 
			// sposto la polylyline oltre che al marker 
			obj.dragMarker( marker ) ;
		}); 
		google.maps.event.addListener(marker, 'dragstart', function() { 
			// sposto la polylyline oltre che al marker 
			obj.dragMarker( marker ) ;
		}); 
		google.maps.event.addListener(marker, 'dragend', function() { 
			// sposto la polylyline oltre che al marker 
			obj.dragMarker( marker ) ;
		}); 		
	} ;
	// aggiungo il marker alla lista 
	this.marker_list.push( marker ) ;
	
}
	
	// questa funzione cancella l' ultimp punto inserito 
RulerMap.prototype.deleteLastDistanceMarker = function(){
	// controllo se c' e un punto da cancellare 
	if ( this.marker_list.length == 0 ) return ;
	// tolgo l' ultimo punto
	var marker = this.marker_list[this.marker_list.length - 1] ;
	// lo tolgo dalla mappa 
	marker.setMap(null);
	// lo tolgo dalla lista 
	this.marker_list.pop() ;
	// conotrollo se era presente una polyllyine collegata 
	if ( this.polyline_list.length > 0 ){
		// tolgo la polylyline collegata 
		var polyline = this.polyline_list[this.polyline_list.length - 1] ;
		// la tolgo dalla mappa 
		polyline.setMap(null);
		// la tolgo dalla lista 
		this.polyline_list.pop() ;
		// tolgo l' etichetta 
		var label = this.label_list[this.label_list.length - 1] ;
		// la tolgo dalla mappa 
		label.setMap(null);
		// la tolgo dalla lista 
		this.label_list.pop() ;			
	}
} ;
	
	// questa funzione cancella tuttti i marker inseriti fino ad adesso 
RulerMap.prototype.deleteAllDistanceMarker = function(){
	// controllo tutta la lista di marker 
	for ( var i = 0 ; i < this.marker_list.length ; i++ ){
		// tolgo il marker corrente dalla mappa 
		this.marker_list[i].setMap(null);
	}
	// pulisco la lista 
	this.marker_list = [] ;
	// tolgo le polyline 
	for ( var i = 0 ; i < this.polyline_list.length ; i++ ){
		this.polyline_list[i].setMap(null);
	}
	// pulisco la lista 
	this.polyline_list = [] ;
	// tolgo le etichette  
	for ( var i = 0 ; i < this.label_list.length ; i++ ){
		this.label_list[i].setMap(null);
	}
	this.label_list = [] ;			
} ;


