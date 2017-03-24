<?php

//*************************************************************************************************
// BOITE A OUTILS                                                                                 *
//*************************************************************************************************


//=====================================================
// Fonction d'affichage d'erreurs
//=====================================================

function error($ls_msg)
{
  return("<CENTER><TABLE><TR><TH>ERREUR</TH></TR><TR><TD><FONT COLOR=red>".htmlentities($ls_msg)."</FONT></TD></TR></TABLE></CENTER>") ;
}


//=====================================================
// Fonction de génération de SelectBox
//=====================================================

function ListeDeChoix ($sql, $name, $DefaultValue ="",$attributes = "")
{
  global $conn ;

  // on execute la requete
  $rs = $conn->Execute($sql) ;
  if (!$rs)
  {
    echo $conn->ErrorMsg() ;
    return(array("", "Erreur MySQL:" . $conn->ErrorMsg())) ;
  }

  // on commence à construire la selectbox
  $htmlString = "<SELECT NAME='".$name."' " . $attributes . ">" ;

  // parcours de tous les enregistrements
  while (!$rs->EOF)
  {
    if ($DefaultValue == "" )
    {
      if ($rs->fields[2] == 'oui')
      {
        $select="SELECTED" ;
      }
      else
      {
        $select="" ;
      }
    }
    else
    {
      if ($rs->fields[1] == $DefaultValue)
      {
        $select="SELECTED" ;
      }
      else $select="" ;
    }
    $htmlString .="<OPTION value='".$rs->fields[1]."' ".$select." >".$rs->fields[0]."</OPTION>\n" ;
    $rs->MoveNext() ;
  }

  // on finit de construire la selectbox
  $htmlString .= "</SELECT>" ;

  return $htmlString ;
}


//=====================================================
// Fonction de génération de boutons radio
//=====================================================

function BoutonsRadio ($sql, $name, $DefaultValue ="", $img)
{
  global $conn;

  // on execute la requete
  $rs = $conn->Execute($sql) ;
  if (!$rs)
  {
    echo $conn->ErrorMsg() ;
    return(array("", "Erreur MySQL:" . $conn->ErrorMsg())) ;
  }

  // parcours de tous les enregistrements
  while (!$rs->EOF)
  {
    if ($DefaultValue == "")
    {
      if ($rs->fields[2] == 'oui')
      {
        $select="CHECKED" ;
      }
      else
      {
        $select="" ;
      }
    }
    else
    {
      if ($rs->fields[1] == $DefaultValue)
      {
        $select="CHECKED" ;
      }
      else
      {
        $select="" ;
      }
    }
    $htmlString .="<INPUT type=radio name=".$name." value='".$rs->fields[1]."' ".$select." >&nbsp;<IMG src='".$rs->fields[3]."'>&nbsp;".$rs->fields[0]."\n" ;
    $rs->MoveNext() ;
  }

  return $htmlString ;
}

//=====================================================
// Fonction de vérification d'un droit 
// pour l'utilisateur connecté.
//=====================================================

function a_le_droit ($lib_droit) 
{
  global $conn;
  $sql = "SELECT d.acces FROM droits_acces d, rubrique r WHERE r.id = d.idrubrique AND d.idutilisateur = ".$_SESSION["id_utilisateur"]." AND r.lib = '".$lib_droit."'";
  // on execute la requete
  $rs = $conn->Execute($sql);
  if (!$rs)
  {
    echo $conn->ErrorMsg() ;
    return false;
  }
  if ($rs->fields[0] == 'oui') { return true; }
  else { return false; }
}


//=====================================================
// Fonction de génération de boutons de formulaire
//=====================================================

function Bouton ($libelle="Valider", $fonction="submit", $attributes = "")
{
  $htmlString = "<input type='".$fonction."' NAME='".$name."' value='".$libelle."' " . $attributes . ">" ;
  return $htmlString ;
}


//=====================================================
// Fonction de génération de boutons retour
//=====================================================

function Bouton_Retour ($url = "index.php")
{
	$libelle = " &#139;&#139;&nbsp;Retour";
    // $htmlString  = "<BR><BR><A HREF='".$url."'>".$libelle."</A><BR><BR>";
    $htmlString  = "<input type='button' value='".$libelle."' onClick='location.href=\"".$url."\";' />";
    return $htmlString ;
}

//=====================================================
// Fonction de génération de l'indication 
// de champ obligatoire : *
//=====================================================

function chpobli ()
{
  return "<span class=\"obligatoire\">&nbsp;*&nbsp;</span>";
}

//=====================================================
// Fonction de customisation 
// si pas, utilisation du default
//=====================================================

function get_my_custom ($Custnom, $CustomBase)
{
	if (file_exists("custom/".$CustomBase."/".$Custnom)) return "custom/".$CustomBase."/".$Custnom;
	else if (file_exists("custom/default/".$Custnom)) return "custom/default/".$Custnom;
	else return $Custnom;
}


//=====================================================
// Fonction d'affichage des images par défaut 
// ou personnalisée, pour entete, bas de page et logo.
//=====================================================

function get_my_image ($nom)
{
  if (file_exists("images/custom/".$nom)) return "images/custom/".$nom;
  else return "images/".$nom;
}
?>
