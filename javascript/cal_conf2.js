
//Define calendar(s): addCalendar ("Unique Calendar Name", "Window title", "Form element's name", Form name")
addCalendar("Calendrier", "Choisissez une date", "date", "saisie");     // formulaire de saisie
addCalendar("Calendrier2", "Choisissez une date", "date1", "consult");  // formulaire de consultation, date1
addCalendar("Calendrier3", "Choisissez une date", "date2", "consult");  // formulaire de consultation, date2
addCalendar("Calendrier4", "Choisissez une date", "date1", "states_formulaire");   // formulaire de statistiques, date1
addCalendar("Calendrier5", "Choisissez une date", "date2", "states_formulaire");   // formulaire de statistiques, date2
addCalendar("Calendrier6", "Choisissez une date", "p_date", "formul");   // formulaire d'administration, destruction de données


// default settings for English
// Uncomment desired lines and modify its values
// setFont("verdana", 9);
 setWidth(90, 1, 15, 1);
// setColor("#cccccc", "#cccccc", "#ffffff", "#ffffff", "#333333", "#cccccc", "#333333");
// setFontColor("#333333", "#333333", "#333333", "#ffffff", "#333333");
 setFormat("dd/mm/yyyy");
// setSize(200, 200, -200, 16);

// setWeekDay(0);
 setMonthNames("Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre");
 setDayNames("Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi");
// setLinkNames("[Close]", "[Clear]");
