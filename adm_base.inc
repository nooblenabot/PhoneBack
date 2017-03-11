<?php

//=============DESTRUCTION DES DONNES===============================

//------------------------------------------------------------------
//    Fonction qui affiche le formulaire de destruction de données de la base

function administration_destruction_formulaire ($date)
{
  $barre_bouton = Barre_de_boutons_BO("Supprimer","Effacer","Retour");
  $html = <<<END

  <!-- Vérification du format de date de destruction -->

  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_datedestruct()
    {
      var datedestruct = window.document.forms["formul"].elements["p_date"].value ;
      if  (datedestruct.search(/^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$/) == -1)
        {
          alert("Veuillez saisir un format de date valide : JJ/MM/AAAA");
          return false;
        }
      else
      {
        if (confirm("Etes-vous sûr de vouloir détruire les données de la base ?")== true )
        {
          return true ;
        }
        else
        {
          return false ;
        }
      }
    }
  </SCRIPT>
  
  <!-- JAVASCRIPTS nécessaires pour le calendrier -->
  <script language="javascript" src="javascript/cal2.js"></script>
  <script language="javascript" src="javascript/cal_conf2.js"></script>

  <H2>Je veux d&eacute;truire tout ce qui est ant&eacute;rieur &agrave; la date :</H2>

  <CENTER>
    <FORM METHOD="GET" name="formul" onSubmit="return verifie_datedestruct()">
      <INPUT TYPE="hidden" NAME="action" VALUE="administrer.detruire">
      <INPUT TYPE="text" SIZE="10" NAME="p_date">&nbsp;&nbsp;JJ/MM/AAAA&nbsp;&nbsp;&nbsp;<IMG src="images/calendrier.jpg" align="middle" onMouseUp="javascript:showCal('Calendrier6')">
      <BR><BR>
        $barre_bouton
    </FORM>
  </CENTER>

END;

  return(array($html, null)) ;
}



//------------------------------------------------------------------
//    Fonction qui détruit les données de la base

function detruire($date)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  // on formate la date pour la base de données
  $date_split = explode('/',$_REQUEST["p_date"]) ;
  $dateformatee = mktime (0, 0, 0, $date_split[1], $date_split[0], $date_split[2]) ;

  $sql = "DELETE FROM appel WHERE date_appel < " . $conn->DBTimeStamp($dateformatee) ;
  $res = $conn->Execute($sql) ;

  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  if ($res == true)
  {
    $redirect = true ;
    $action = "administrer" ;
    $footer_msg = "Les donn&eacute;es ont bien &eacute;te supprim&eacute;es de la base" ;
  }
  else
  {
    $redirect = true ;
    $action = "administrer" ;
    $footer_msg = "La suppression n'a pas correctement &eacute;te effectu&eacute;e" ;
  }
}


?>