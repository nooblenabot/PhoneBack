<?php

//*************************************************************************************************
// CONTROLEUR PRINCIPAL                                                                           *
// DE L'APPLICATION                                                                               *
//*************************************************************************************************



// Utilisation des variables de session
session_start() ;


// Inclusion des fichiers nécéssaires à l'application
include "config.inc" ;
include "header.inc" ;
include "adodb/adodb.inc.php" ;
include "function.inc" ;
include "identification.inc" ;
include "saisie.inc" ;
include "consultation.inc" ;
include "states.inc" ;
include "administration.inc" ;
include "panier.inc" ;


// Connexion
$conn  = &ADONewConnection(BASE_TYPE) ;  // create a connection
$conn->debug=0 ;
$conn->PConnect(SERVER, ACCOUNT, PASSWORD, BASE) ; // connect to MySQL, testdb


// Variables globales
$action = null ;
$footer_msg = "" ;
$msg_mail = "" ;
$redirect = false ;



//==============================================================
// programme principal
//==============================================================

if ( (!isset ($_REQUEST["action"])) || ( (!isset ($_SESSION["id_utilisateur"])&&($_REQUEST["action"]!="identifier.verif")) ) )
{
  // on détruit toutes les variables de session
  session_unset() ;
  // on détruit la session
  session_destroy() ;
  // action par défaut
  $action = "identifier" ;
}
else
{
  $action = $_REQUEST["action"] ;
}


do
{
  $actions = explode('.', $action) ;
  $redirect = false;
  switch($actions[0])
  {
    //-------------------------------------------------------------------------------------------
    case "identifier":
        // sous switch de l'action identifier
        identifier_switch($action) ;
      break;
    //-------------------------------------------------------------------------------------------
    default :
      if ($_SESSION["id_utilisateur"])
      {
        // affichage du menu uniquement si l'utilisateur est identifié
        include_once "menu.inc";
        
        // Gestion des rubriques
        switch($actions[0])
        {
	    case "saisir":
	        // sous switch de l'action saisir
	        if (a_le_droit("SAISIE")) saisir_switch($action);
	        else include_once "msg_droits.inc";
	      break;
	    //-------------------------------------------------------------------------------------------
	    case "consulter":
	        // sous switch de l'action consulter
	        if (a_le_droit("CONSULTATION")) consulter_switch($action);
	        else include_once "msg_droits.inc";
	      break;
	    //-------------------------------------------------------------------------------------------
	    case "states":
	        // sous switch de l'action states
	        if (a_le_droit("STATISTIQUES")) states_switch($action);
	        else include_once "msg_droits.inc";
	      break;
	    //-------------------------------------------------------------------------------------------
	    case "administrer":
	        // sous switch de l'action administrer
	        if (a_le_droit("ADMINISTRATION")) administrer_switch($action);
	        else include_once "msg_droits.inc";
	      break;
	    //-------------------------------------------------------------------------------------------
	    case "panier":
	        // sous switch de l'action panier
	        if (a_le_droit("STATISTIQUES")) panier_switch($action);
	        else include_once "msg_droits.inc";
	      break;
	    //-------------------------------------------------------------------------------------------
	    default:
	      echo "ACTION $action n'est pas définie dans l'application\n" ;
  	    }
  	  }
	  else
	  {
	    $action = "identifier" ;
	    $redirect = true ;
	  }
  }
}

// affichage des messages d'erreur
while ($redirect == true ) ;
{
  include("msg_erreurs.inc") ;
}

if ($action != "identifier")
{
  // affichage de l'utilisateur de l'application
  include ("msg_utilisateur.inc") ;
  // affichage du menu bas
  include "footer.inc" ;
}

//phpinfo();

?>
