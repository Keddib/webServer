<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the
 * installation. You don't have to use the web site, you can
 * copy this file to "wp-config.php" and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * MySQL settings
 * * Secret keys
 * * Database table prefix
 * * ABSPATH
 *
 * @link https://wordpress.org/support/article/editing-wp-config-php/
 *
 * @package WordPress
 */

// ** MySQL settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define( 'DB_NAME', 'wordpress' );

/** MySQL database username */
define( 'DB_USER', 'wp-user' );

/** MySQL database password */
define( 'DB_PASSWORD', 'wp-pass' );

/** MySQL hostname */
define( 'DB_HOST', 'localhost' );

/** Database Charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8' );

/** The Database Collate type. Don't change this if in doubt. */
define( 'DB_COLLATE', '' );

/**#@+
 * Authentication Unique Keys and Salts.
 *
 * Change these to different unique phrases!
 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}
 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.
 *
 * @since 2.6.0
 */

define('AUTH_KEY',         'yQ(>IDqQ/]U{-V]A`]w6@}7R|jJ|P|Q4P-Cikw^#Lwd|!-(m%aK%mq8?}-yyhU4M');
define('SECURE_AUTH_KEY',  '>H]7~,mRnOB0I`kO]Ub,-5Ga}{Rezv?/xLe$H6B|t~=Wd#UC8fy8oy10aNy?j9c=');
define('LOGGED_IN_KEY',    '+gu@XE8ga.a(LKpPkCFgf(z?g6rym# f~tg/ ^QY+_[g]=fYTeoWZ]DS.$28$B,~');
define('NONCE_KEY',        '4[~%L>z =DldJ:d+Ml ++|Hzqn|?3EUrVSP`g(Utp++:MZ9u UeMDwjtg)Q6.J6f');
define('AUTH_SALT',        '9|Ip~JiE)cW*7[,pY$F,yy(Yo8a|WXF6w$)K 0vGM3vd-x[Eg9A8CEr985;pX]7:');
define('SECURE_AUTH_SALT', '<+B;H7Y+e9>`:k6e$dd U_d{2 =|q|V]G-$-C,p;|-Abp}y2[d5<h5`^+*G/Y(VS');
define('LOGGED_IN_SALT',   '}Hv=|ciYpRQ)DRUqVMhh!Y#|N{$L>v7Z#|-%rzB,^Kb~ vBK`onC=V|lud~LD:tY');
define('NONCE_SALT',       '!a5& *ABNG[-fqRs@-:UQDuKG+k|SZj=x{*gMnCYB_&%$Iu3K[Nm,^qyawPQMRy=');



/**#@-*/

/**
 * WordPress Database Table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 */
$table_prefix = 'wp_';

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 *
 * For information on other constants that can be used for debugging,
 * visit the documentation.
 *
 * @link https://wordpress.org/support/article/debugging-in-wordpress/
 */
define( 'WP_DEBUG', false );

/* That's all, stop editing! Happy publishing. */

/** Absolute path to the WordPress directory. */
if ( ! defined( 'ABSPATH' ) ) {
	define( 'ABSPATH', __DIR__ . '/' );
}

/** Sets up WordPress vars and included files. */
require_once ABSPATH . 'wp-settings.php';
