<?php
//=============GESTION DES UTILISATEURS=============================

//------------------------------------------------------------------
//    Fonction qui affiche tous les utilisateurs de l'application
function affiche_liste_utilisateurs()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  $barre_bouton = Barre_de_boutons_BO("","","Retour");  

  $sql = "SELECT id, nom, prenom FROM utilisateur ORDER BY nom" ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  else
  {
    echo "<CENTER>" ;
    echo "<H2>Utilisateurs de l'application :</H2>" ;
    echo "<IMG SRC=\"images/utilisateur.gif\">&nbsp;<A HREF=\"index.php?action=administrer.afficheformulaire\">Ajouter un nouvel utilisateur</A>" ;
    echo "<BR><BR>" ;
    echo "<TABLE class=\"RESULTAT\"><TR><TH>Nom</TH><TH>Pr&eacute;nom</TH><TH colspan=\"2\">Actions</TH></TR>" ;
    while (!$res->EOF)
    {
      echo "<TR>
                <TD>".$res->fields[1]."</TD><TD>".$res->fields[2]."</TD>
                <TD align=\"center\"><A HREF=index.php?action=administrer.modifieutilisateur&idutilisateur=".$res->fields[0]."&choix=2><IMG SRC=\"images/edit.gif\" border=0></A></TD>
                <TD align=\"center\"><A HREF=index.php?action=administrer.supprimeutilisateur&idutilisateur=".$res->fields[0]."&choix=2><IMG SRC=\"images/delete.gif\" border=0></A></TD>
            </TR>" ;
      $res->MoveNext() ;
    }
    echo "</TABLE><br><br>" ;
	echo $barre_bouton;
    echo "</CENTER>" ;
  }

}

//------------------------------------------------------------------
//    Fonction qui affiche le formulaire de remplissage
//    des données propres à l'utilisateur

function administration_ajoututilisateur_formulaire($nom="", $prenom="", $login="", $motpasse="", $motpasse2="", $mail="", $idutilisateur="", $Profil="")
{
  $etoile = chpobli();
  if ($idutilisateur=="")
  {
    $myaction="administrer.insertutilisateur";
    $barre_bouton = Barre_de_boutons_BO("Insérer","Effacer","Retour");
  }
  else
  {
    $myaction="administrer.applimodifsutilisateur";
    $barre_bouton = Barre_de_boutons_BO("Modifier","Effacer","Retour");
  }

  $html = <<<END

    <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      var nomutil = window.document.forms["infosutilisateur"].elements["p_nom"].value;
      var prenomutil = window.document.forms["infosutilisateur"].elements["p_prenom"].value;
	  var Profil = window.document.forms["infosutilisateur"].elements["p_profil"].value;
      var loginutil = window.document.forms["infosutilisateur"].elements["p_login"].value;
      var motpasseutil1 = window.document.forms["infosutilisateur"].elements["p_motpasse1"].value;
      var motpasseutil2 = window.document.forms["infosutilisateur"].elements["p_motpasse2"].value;
      var mailutil = window.document.forms["infosutilisateur"].elements["p_mail"].value;
      var place = mailutil.indexOf("@");

      // vérification que chaque champ est bien renseigné
      if ( (nomutil.length==0) || (prenomutil.length==0) || (loginutil.length==0)
        || (motpasseutil1.length==0) || (motpasseutil2.length==0) || (mailutil.length==0) || (profil.length==0) )
      {
        alert ("Vous avez oublié de remplir un champ !" ) ;
        return false ;
      }
      // vérification que l'arobase se trouve dans le champ email,
      //              que l'arobase n'est pas en première ou en dernière position de la chaine de caractères
      if ( (place == -1) || (place == 0) || (place == ((mailutil.length)-1)) )
      {
        alert ("Veuillez entrer une adresse e-mail valide !") ;
        return false ;
      }
      // vérification que les deux mots de passe sont identiques
      if ( motpasseutil1!=motpasseutil2 )
      {
        alert ("Attention, vos deux mots de passe sont différents !") ;
        return false ;
      }
      else
      {
        return true ;
      }
    }
  </SCRIPT>


    <FORM method="GET" name="infosutilisateur" onSubmit="return verifie_formulaire()">

      <H2>Veuillez saisir ou modifier les informations utilisateur :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD><INPUT TYPE="hidden" NAME="action" VALUE="$myaction"></TD>
          <TD><INPUT TYPE="hidden" NAME="idutilisateur" VALUE="$idutilisateur"></TD>
          <TD><INPUT TYPE="hidden" NAME="choix" VALUE="2"></TD>
        </TR>
        <TR>
          <TD>Nom$etoile</TD>
          <!--                             on change les lettre tapées en minuscules en lettres majuscules -->
          <TD><INPUT type="text" name="p_nom" value="$nom" onChange="javascript:this.value=this.value.toUpperCase();"></TD>
        </TR>
        <TR>
          <TD>Pr&eacute;nom$etoile</TD>
          <TD><INPUT type="text" name="p_prenom" value="$prenom"></TD>
        </TR>
		<TR>
          <TD>Profil$etoile</TD>
          <TD><INPUT type="text" name="p_profil" value="$Profil"></TD>
        </TR>
        <TR>
          <TD>Login$etoile</TD>
          <TD><INPUT type="text" name="p_login" value="$login"></TD>
        </TR>
        <TR>
          <TD>Mot de passe$etoile</TD>
          <TD><INPUT type="password" name="p_motpasse1" value="$motpasse"></TD>
        </TR>
        <TR>
          <TD>Retapez le mot de passe</TD>
          <TD><INPUT type="password" name="p_motpasse2" value="$motpasse2"></TD>
        </TR>
        <TR>
          <TD>Adresse mél$etoile</TD>
          <TD><INPUT type="text" name="p_mail" value="$mail"></TD>
        </TR>
      </TABLE>
      <BR>
      <CENTER>
			$barre_bouton
      </CENTER>
    </FORM>
END;

  return(array($html, null)) ;
}

//------------------------------------------------------------------
//    Fonction qui insère le nouvel utilisateur dans la base

function insertutilisateur($p_nom,$p_prenom,$p_login,$p_motpasse,$p_mail,$profil)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql =  "INSERT INTO utilisateur ( nom, prenom, login, motpasse, mail, Profil ) ".
          "VALUES (".$conn->qstr($p_nom).",".$conn->qstr($p_prenom).",".$conn->qstr($p_login).",".
                     $conn->qstr($p_motpasse).",".$conn->qstr($p_mail).",".$conn->qstr($p_profil).")" ;
  $res = $conn->Execute($sql) ;
  
  // Initialisation de ses droits sur l'application
  $res = init_droits($conn->Insert_ID());
  
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  if ($res == true)
  {
    $footer_msg = "Le nouvel utilisateur a bien &eacute;te ins&eacute;r&eacute; dans la base" ;
  }
  else
  {
    $footer_msg = "Il y a eu un probl&egrave;me lors de l'insertion dans la base" ;
  }  
  $redirect = true ;
  $action = "administrer.choix" ;
  
}

//------------------------------------------------------------------
//    Fonction qui va modifier la fiche d'un utilisateur

function modifie_utilisateur($nom, $prenom, $login, $motpasse, $mail, $profil, $idutilisateur)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "UPDATE utilisateur SET nom=".$conn->qstr($nom).", prenom=".$conn->qstr($prenom).", login=".$conn->qstr($login).", motpasse=".$conn->qstr($motpasse).", mail=".$conn->qstr($mail).", Profil=".$conn->qstr($profil)." WHERE id=".$idutilisateur ;
  $res = $conn->Execute($sql);
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  if ($res==true)
  {
    $footer_msg = "Les donn&eacute;es ont bien &eacute;t&eacute; modifi&eacute;es." ;
  }
  else
  {
    $footer_msg = "Vos donn&eacute;es n'ont pas &eacute;t&eacute; modifi&eacute;es, veuillez recommencer" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}

//------------------------------------------------------------------
//    Fonction qui supprime un utilisateur de la base

function supprime_utilisateur()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  //supprimer tous les droits
  $res = supprimer_droits ($_REQUEST["idutilisateur"]);
  
  // supprimer l'utilisateur.
  $sql = "DELETE FROM utilisateur WHERE id=".$_REQUEST["idutilisateur"] ;
  $res = $conn->Execute($sql) ;
  
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  if ($res==true)
  {
    $footer_msg = "L'utilisateur a bien &eacute;te d&eacute;truit de la base" ;
  }
  else
  {
    $footer_msg = "L'utilisateur n'a pas &eacute;t&eacute; d&eacute;truit de la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}

?>