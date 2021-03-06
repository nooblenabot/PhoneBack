<?php

//*************************************************************************************************
// FICHIER DE CONFIGURATION GENERALE                                                              *
// POUR INSTALLATION SUR LE SERVEUR                                                               *
//*************************************************************************************************

// modifier php.ini avec celui contenu dans les dossier php ini
// cf "notice" � mettre en position OFF
// cf activation de la librairie GD...

//=====================================================
// CHEMINS DE L'APPLICATION
// un chemin sera du type :
// http://SERVER_NAME/SCRIPT_NAME/index.php
//=====================================================
define (SCRIPT_NAME,(substr($_SERVER["SCRIPT_NAME"],1, (strrpos($_SERVER["SCRIPT_NAME"],'/'))-1))) ;
define (SERVER_NAME, ($_SERVER["SERVER_NAME"])) ;


//=====================================================
// CONNEXION A LA BASE       (index.php)
//=====================================================
define (BASE_TYPE, 'mysql') ;
define (SERVER, 'localhost') ;
define (ACCOUNT, 'appels') ;
define (PASSWORD, "tmEJPzWkYUEi72GV") ;
define (BASE, 'appels') ;


//=====================================================
// ENVOI DE MAIL
//=====================================================
// voir insert.inc
// remplacer par le mail de l'utilisateur :   $mail_utilis
// remplacer par le mail du destinataire :    $mail_desti

// configurer le serveur de mail :
// cf php.ini
// [mail function]
// ; For Win32 only.
// SMTP = mail.erasme.org
// ; For Win32 only.
// sendmail_from = support@erasme.org


//=====================================================
// LIBRAIRIE GD
//=====================================================
// Attention, elle est impl�ment�e sous EasyPHP mais pas forc�ment sous Linux.
// V�rifier donc que le serveur ne n�cessite pas une installation de cette librairie

// Sous Linux il faut r�cup�rer la librairie � l'adresse www.boutell.com/gd
// l'installer sur la machine et configurer PHP avec l'option --with-gd[=insGD]


//=====================================================
// JPGRAPH
//=====================================================
// revoir jpgraph/jpgraph.php (chemin depuis la racine du site)
// o� a �t� mise la configuration Windows :
//    define (TTF_DIR, 'C:\\winnt\fonts\') ;


//=====================================================
// FPDF
// D�finition du chemin pour les fonts
//=====================================================
define('FPDF_FONTPATH','pdf/font/');


//=====================================================
// DUREE DE LA SESSION SI INACTIVE
//=====================================================

?>
