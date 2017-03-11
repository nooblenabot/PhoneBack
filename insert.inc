<?php

//============================================================
// FONCTIONS
//============================================================


//------------------------------------------------------------------
// Fonction d'insertion de l'appel dans la base

function insert($date, $heure, $structure, $lieu, $appelant, $humeur, $type, $destinataire, $issue, $notif, $numero, $commentaire)
{
  global $conn ;
  global $msg_mail ;
  
  // gestion des dates au format de la base
  $date_split = explode('/',$date) ;
  $time_split = explode(':',$heure) ;
  $date_time = mktime ($time_split[0], $time_split[1], $time_split[2], $date_split[1], $date_split[0], $date_split[2]) ;


  //=====================================================================
  // table LIEU
  //=====================================================================

  $lieu_id = null;

  $sql = "SELECT id FROM lieu WHERE nom LIKE ". $conn->qstr($lieu) ;
  $res = $conn->Execute($sql) ;

  // gère les erreurs
  if (!$res)
  {
    print "Error 1: " . $conn->ErrorMsg() ;
    die;
  }

  while (!$res->EOF)
  {
    $lieu_id = $res->fields[0] ;
    $res->MoveNext() ;
  }

  // on n'insère un nouveau lieu dans la base que s'il n'existe pas
  if (!isset($lieu_id))
  {
    $sql = " INSERT INTO lieu ( nom ) VALUES ( " . $conn->qstr($lieu) . " ) " ;
    $res = $conn->Execute($sql) ;
    if (!$res)
    {
      print "Error 2: " . $conn->ErrorMsg() ;
      die;
    }
    $lieu_id = $conn->Insert_ID() ;
  }


  //=====================================================================
  // Table PERSONNE
  //=====================================================================

  $personne_id = null;

  $sql = "SELECT id FROM personne WHERE nom LIKE " . $conn->qstr($appelant) ;
  $res = $conn->Execute($sql) ;

  // gère les erreurs
  if (!$res)
  {
    print "Error 3: " . $conn->ErrorMsg() ;
    die;
  }

  while (!$res->EOF)
  {
    $personne_id = $res->fields[0] ;
    $res->MoveNext() ;
  }

  // on n'insère une nouvelle personne dans la base que si elle n'existe pas
  if (!isset($personne_id))
  {
    $sql = "INSERT INTO personne ( nom ) VALUES ( " . $conn->qstr($appelant)." )" ;
    $res = $conn->Execute($sql);

    if (!$res)
    {
      print "Error 4: " . $conn->ErrorMsg() ;
      die;
    }
    $personne_id = $conn->Insert_ID() ;
  }


  //=====================================================================
  // table APPEL
  //=====================================================================

  $sql =  "INSERT INTO appel ( date_appel, envoimail, commentaire, tel, id_humeur, id_lieu, id_structure, id_issue, id_destinataire, id_type, id_utilisateur, id_personne ) ".
          "VALUES ( " . $conn->DBTimeStamp($date_time) . " , '$notif', " . $conn->qstr($commentaire) . ", '$numero', '$humeur', '$lieu_id', '$structure', '$issue', '$destinataire', '$type', ".$_SESSION["id_utilisateur"].", $personne_id )" ;
  $res = $conn->Execute($sql);

  if (!$res)
  {
    print "Error 5: " . $conn->ErrorMsg();
    die;
  }


  //=====================================================================
  // envoi mail
  //=====================================================================

  if ($notif=="oui")
  {
    $sql1 = "SELECT prenom, nom, mail FROM utilisateur WHERE id=".$_SESSION["id_utilisateur"] ;
    $sql2  = "SELECT prenom, nom, mail FROM destinataire WHERE id=".$destinataire ;
    $sql3 = "SELECT nom FROM structure WHERE id=".$structure ;
    $sql4 = "SELECT libelle FROM type WHERE id=".$type ;
    $sql5 = "SELECT libelle FROM issue WHERE id=".$issue ;
    //$sql6 = "SELECT libelle FROM humeur WHERE id=".$humeur ;

    $res1 = $conn->Execute($sql1) ;
    $res2 = $conn->Execute($sql2) ;
    $res3 = $conn->Execute($sql3) ;
    $res4 = $conn->Execute($sql4) ;
    $res5 = $conn->Execute($sql5) ;
    //$res6 = $conn->Execute($sql6) ;
    
    if (!$res1 || !$res2 || !$res3 || !$res4 || !$res5  )
    {
      print "Error 4: " . $conn->ErrorMsg() ;
      die ;
    }
    else
    {
      $prenom_utilis = $res1->fields[0] ;
      $nom_utilis = $res1->fields[1] ;
      $mail_utilis = $res1->fields[2] ;
      $prenom_desti = $res2->fields[0] ;
      $nom_desti = $res2->fields[1] ;
      $mail_desti = $res2->fields[2] ;
      $nom_struct = $res3->fields[0] ;
      $nom_type = $res4->fields[0] ;
      $issue = $res5->fields[0] ;
      //$humeur = $res6->fields[0] ;

      // some local variables
      // pour encoder les caractères spéciaux
      $from_name = "=?iso-8859-1?B?".base64_encode($prenom_utilis." ".$nom_utilis)."?=" ;
      $from_email = $mail_utilis ;
      $to_name = "=?iso-8859-1?B?".base64_encode($prenom_desti." ".$nom_desti)."?=" ;
      $to_email = $mail_desti ;
      $subject = "=?iso-8859-1?B?".base64_encode("Vous avez reçu un appel de ".$appelant."")."?=" ;

      // headers need to be in the correct order...
      $headers = "From: $from_name<$from_email>\n";
      $headers .= "Reply-To: <$from_email>\n";
      $headers .= "MIME-Version: 1.0\n";
      $headers .= "Date: ".date("l j F Y, G:i")."\n";
	  
      // the following must be one line (post width too small)
      $headers .= "Content-Type: text/plain ; charset=\"iso-8859-1\"\n" ;

      //multipart/related;
      //type=\"multipart/alternative\"; boundary=\"----
      //=MIME_BOUNDRY_main_message\"\n";

      $headers .= "X-Sender: $from_name<$from_email>\n";
      $headers .= "X-Mailer: PHP4\n"; //mailer
	  
      $headers .= "X-Priority: 3\n"; //1 UrgentMessage, 3 Normal
      $headers .= "Return-Path: <$from_email>\n";
      //$headers .= "This is a multi-part message in MIME format.\n";
      //$headers .= "------=MIME_BOUNDRY_main_message \n";
      //$headers .= "Content-Type: multipart/alternative; boundary=\"----=MIME_BOUNDRY_message_parts\"\n";

      //plaintext section begins
      //$message = "------=MIME_BOUNDRY_message_parts\n";
      //$message .= "Content-Type: text/plain; charset=\"iso-8859-1\"\n";
      //$message .= "Content-Transfer-Encoding: quoted-printable\n";
      //$message .= "\n";

      $message = "Vous avez reçu un appel durant votre absence : \n" ;
      $message .= "   Nom : ".$appelant."\n   Type de structure : ".$nom_struct."\n" ;
      $message .= "   Lieu : ".$lieu."\n   Tel : ".$numero."\n" ;
      $message .= "   Type d'appel : ".$nom_type."\n" ;
      $message .= "   Date et heure de l'appel : ".$date." , ".$heure."\n";
      $message .= "   Issue de l'appel : ".$issue."\n" ;
      //$message .= "   Humeur lors de l'appel : ".$humeur."\n\n" ;
      $message .= "Commentaire :\n";
      $message .= $commentaire."\n\n" ;
      $message .= "====================\n" ;
      $message .= "A bientôt.\n\n";
      $message .= $prenom_utilis."\n" ;
      //$message .= "====================\n" ;
      //$message .= "Destinataire de l'appel : ".$mail_desti ;

      // your text goes here
      //$message .= "blah blah -- plaintext version of the message\n";
      //$message .= "\n";

      // html section begins
      //$message .= "------=MIME_BOUNDRY_message_parts\n";
      //$message .= "Content-Type: text/html;\n    charset=\"iso-8859-1\"\n";
      //$message .= "Content-Transfer-Encoding: quoted-printable\n";
      //$message .= "\n";

      // your html goes here -- It didn't appear properly without
      // the weird markup that outlook added after sending
      //$message .= "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n";
      //$message .= "<HTML><BODY>\n";

      // look ma, I'm referencing an img attachment (see below)
      // watch out for weird markup!!!
      //$message .= "<IMG src=3D\"cid:some_picture\">\n";
      //$message .= "</BODY></HTML>\n";
      //$message .= "\n";

      // this ends the message part
      //$message .= "------=MIME_BOUNDRY_message_parts--\n";
      //$message .= "\n";

      // now we add attachments (images, etc)
      //$message .= "------=MIME_BOUNDRY_main_message\n";
      //$message .= "Content-Type: image/gif; \n name=\"some_picture.gif\"\n";
      //$message .= "Content-Transfer-Encoding: base64\n";
      //$message .= "Content-ID: <some_picture>\n";
      //$message .= "\n";
      // (truncated for space)
      //$message .= "R0lGODlheAAZAKIHAMTExCQkJJOTk\n";
      //$message .= "eLo7wzDKSatVQ5R3u7dDUUjcZ34D\n";
      //$message .= "\n";
      // etc...etc...etc...

      //message ends
      //$message .= "------=MIME_BOUNDRY_main_message--\n";

      // send the message :-)
      if (mail("$to_name<$to_email>", $subject, $message, $headers))
      {
        $msg_mail = "Votre message a bien été envoyé" ;
      }
      else
      {
        $msg_mail = "Il y a eu un problème lors de l'envoi de votre message" ;
      }
    }
  }
  return $res ;
}

?>