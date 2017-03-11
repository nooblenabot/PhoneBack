<?php

//*************************************************************************************************
// SOUS-CONTROLEUR DE L'IDENTIFICATION                                                            *
// + fonction d'affichage du formulaire d'identification                                          *
// + fonction de test d'identification                                                            *
//*************************************************************************************************



//==================================================================
// FONCTIONS
//==================================================================


//------------------------------------------------------------------
// Fonction d'affichage du formulaire d'identification

function identification_formulaire ($login, $motpasse)
{

  $html = <<<END

  <BR><BR><BR>

  <H1>Veuillez remplir les champs suivants pour vous connecter :</H1>

  <FORM METHOD="POST">
  <TABLE class="RESULTAT">
    <TR>
      <TD><INPUT TYPE="hidden" NAME="action" VALUE="identifier.verif"></TD>
    <TR>
    <TR>
      <TD>Identifiant</TD>
      <TD><INPUT TYPE="text" SIZE="20" NAME="login"></TD>
    </TR>
    <TR>
      <TD>Mot de passe</TD>
      <TD><INPUT TYPE="password" SIZE="20" NAME="motpasse"></TD>
    </TR>
  </TABLE>
  <BR><BR>
  <INPUT TYPE="submit" VALUE="Se connecter">
  </FORM>

END;

  return(array($html, null)) ;
}


//---------------------------------------------------------------------------
// Fonction de vérification du couple login/mot de passe

function identifier($login, $motpasse)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "SELECT id FROM utilisateur WHERE  login=" . $conn->qstr($login) . " AND motpasse=" . $conn->qstr($motpasse) ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  $nb = $res->RowCount("utilisateur") ;
  if (($nb) <> NULL )
  {
    $redirect = true ;
    $action = "consulter" ;
    $_SESSION["id_utilisateur"] = $res->fields[0] ;
  }
  else
  {
    $redirect = true ;
    $action = "identifier" ;
    $footer_msg = "Votre identifiant et/ou votre mot de passe est incorrect !" ;
  }
  return (null) ;
}



//===========================================================================
// PROGRAMME PRINCIPAL
//===========================================================================

function identifier_switch($action)
{
  global $_REQUEST ;


  switch ($action)
  {
    //-------------------------------------------------------------------------------------------
    case "identifier":
      // on affiche le formulaire d'identification
      $res = identification_formulaire ($_REQUEST["login"], $_REQUEST["motpasse"]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]) ;
      }
      break;
    //-------------------------------------------------------------------------------------------
    case "identifier.verif":
      // on vérifie le couple login/mot de passe
      $res = identifier($_REQUEST["login"],$_REQUEST["motpasse"]) ;
      if($res != null)
      {
        print error($res);
      }
      break;
    //-------------------------------------------------------------------------------------------
    default:
      print error("ACTION ".$action." pas definie") ;
  }
}

?>