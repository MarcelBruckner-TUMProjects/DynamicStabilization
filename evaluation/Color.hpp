//
// Created by brucknem on 01.04.21.
//

#ifndef CAMERASTABILIZATION_COLORPALETTE_HPP
#define CAMERASTABILIZATION_COLORPALETTE_HPP
#pragma once

#include <opencv2/opencv.hpp>

class Color {
public:
	static cv::Scalar aliceblue;
	static cv::Scalar antiquewhite;
	static cv::Scalar aqua;
	static cv::Scalar aquamarine;
	static cv::Scalar azure;
	static cv::Scalar beige;
	static cv::Scalar bisque;
	static cv::Scalar black;
	static cv::Scalar blanchedalmond;
	static cv::Scalar blue;
	static cv::Scalar blueviolet;
	static cv::Scalar brown;
	static cv::Scalar burlywood;
	static cv::Scalar cadetblue;
	static cv::Scalar chartreuse;
	static cv::Scalar chocolate;
	static cv::Scalar coral;
	static cv::Scalar cornflowerblue;
	static cv::Scalar cornsilk;
	static cv::Scalar crimson;
	static cv::Scalar cyan;
	static cv::Scalar darkblue;
	static cv::Scalar darkcyan;
	static cv::Scalar darkgoldenrod;
	static cv::Scalar darkgray;
	static cv::Scalar darkgreen;
	static cv::Scalar darkgrey;
	static cv::Scalar darkkhaki;
	static cv::Scalar darkmagenta;
	static cv::Scalar darkolivegreen;
	static cv::Scalar darkorange;
	static cv::Scalar darkorchid;
	static cv::Scalar darkred;
	static cv::Scalar darksalmon;
	static cv::Scalar darkseagreen;
	static cv::Scalar darkslateblue;
	static cv::Scalar darkslategray;
	static cv::Scalar darkslategrey;
	static cv::Scalar darkturquoise;
	static cv::Scalar darkviolet;
	static cv::Scalar deeppink;
	static cv::Scalar deepskyblue;
	static cv::Scalar dimgray;
	static cv::Scalar dimgrey;
	static cv::Scalar dodgerblue;
	static cv::Scalar firebrick;
	static cv::Scalar floralwhite;
	static cv::Scalar forestgreen;
	static cv::Scalar fuchsia;
	static cv::Scalar gainsboro;
	static cv::Scalar ghostwhite;
	static cv::Scalar gold;
	static cv::Scalar goldenrod;
	static cv::Scalar gray;
	static cv::Scalar green;
	static cv::Scalar greenyellow;
	static cv::Scalar grey;
	static cv::Scalar honeydew;
	static cv::Scalar hotpink;
	static cv::Scalar indianred;
	static cv::Scalar indigo;
	static cv::Scalar ivory;
	static cv::Scalar khaki;
	static cv::Scalar lavender;
	static cv::Scalar lavenderblush;
	static cv::Scalar lawngreen;
	static cv::Scalar lemonchiffon;
	static cv::Scalar lightblue;
	static cv::Scalar lightcoral;
	static cv::Scalar lightcyan;
	static cv::Scalar lightgoldenrodyellow;
	static cv::Scalar lightgray;
	static cv::Scalar lightgreen;
	static cv::Scalar lightgrey;
	static cv::Scalar lightpink;
	static cv::Scalar lightsalmon;
	static cv::Scalar lightseagreen;
	static cv::Scalar lightskyblue;
	static cv::Scalar lightslategray;
	static cv::Scalar lightslategrey;
	static cv::Scalar lightsteelblue;
	static cv::Scalar lightyellow;
	static cv::Scalar lime;
	static cv::Scalar limegreen;
	static cv::Scalar linen;
	static cv::Scalar magenta;
	static cv::Scalar maroon;
	static cv::Scalar mediumaquamarine;
	static cv::Scalar mediumblue;
	static cv::Scalar mediumorchid;
	static cv::Scalar mediumpurple;
	static cv::Scalar mediumseagreen;
	static cv::Scalar mediumslateblue;
	static cv::Scalar mediumspringgreen;
	static cv::Scalar mediumturquoise;
	static cv::Scalar mediumvioletred;
	static cv::Scalar midnightblue;
	static cv::Scalar mintcream;
	static cv::Scalar mistyrose;
	static cv::Scalar moccasin;
	static cv::Scalar navajowhite;
	static cv::Scalar navy;
	static cv::Scalar oldlace;
	static cv::Scalar olive;
	static cv::Scalar olivedrab;
	static cv::Scalar orange;
	static cv::Scalar orangered;
	static cv::Scalar orchid;
	static cv::Scalar palegoldenrod;
	static cv::Scalar palegreen;
	static cv::Scalar paleturquoise;
	static cv::Scalar palevioletred;
	static cv::Scalar papayawhip;
	static cv::Scalar peachpuff;
	static cv::Scalar peru;
	static cv::Scalar pink;
	static cv::Scalar plum;
	static cv::Scalar powderblue;
	static cv::Scalar purple;
	static cv::Scalar red;
	static cv::Scalar rosybrown;
	static cv::Scalar royalblue;
	static cv::Scalar saddlebrown;
	static cv::Scalar salmon;
	static cv::Scalar sandybrown;
	static cv::Scalar seagreen;
	static cv::Scalar seashell;
	static cv::Scalar sienna;
	static cv::Scalar silver;
	static cv::Scalar skyblue;
	static cv::Scalar slateblue;
	static cv::Scalar slategray;
	static cv::Scalar slategrey;
	static cv::Scalar snow;
	static cv::Scalar springgreen;
	static cv::Scalar steelblue;
	static cv::Scalar tan;
	static cv::Scalar teal;
	static cv::Scalar thistle;
	static cv::Scalar tomato;
	static cv::Scalar turquoise;
	static cv::Scalar violet;
	static cv::Scalar wheat;
	static cv::Scalar white;
	static cv::Scalar whitesmoke;
	static cv::Scalar yellow;
	static cv::Scalar yellowgreen;

	static cv::Scalar any(cv::RNG &rng) {
		auto a = all();
		int index = rng.uniform(0, a.size());
		return a[index];
	}

	static std::vector<cv::Scalar> all() {
		return {
			aliceblue,
			antiquewhite,
			aqua,
			aquamarine,
			azure,
			beige,
			bisque,
			black,
			blanchedalmond,
			blue,
			blueviolet,
			brown,
			burlywood,
			cadetblue,
			chartreuse,
			chocolate,
			coral,
			cornflowerblue,
			cornsilk,
			crimson,
			cyan,
			darkblue,
			darkcyan,
			darkgoldenrod,
			darkgray,
			darkgreen,
			darkgrey,
			darkkhaki,
			darkmagenta,
			darkolivegreen,
			darkorange,
			darkorchid,
			darkred,
			darksalmon,
			darkseagreen,
			darkslateblue,
			darkslategray,
			darkslategrey,
			darkturquoise,
			darkviolet,
			deeppink,
			deepskyblue,
			dimgray,
			dimgrey,
			dodgerblue,
			firebrick,
			floralwhite,
			forestgreen,
			fuchsia,
			gainsboro,
			ghostwhite,
			gold,
			goldenrod,
			gray,
			green,
			greenyellow,
			grey,
			honeydew,
			hotpink,
			indianred,
			indigo,
			ivory,
			khaki,
			lavender,
			lavenderblush,
			lawngreen,
			lemonchiffon,
			lightblue,
			lightcoral,
			lightcyan,
			lightgoldenrodyellow,
			lightgray,
			lightgreen,
			lightgrey,
			lightpink,
			lightsalmon,
			lightseagreen,
			lightskyblue,
			lightslategray,
			lightslategrey,
			lightsteelblue,
			lightyellow,
			lime,
			limegreen,
			linen,
			magenta,
			maroon,
			mediumaquamarine,
			mediumblue,
			mediumorchid,
			mediumpurple,
			mediumseagreen,
			mediumslateblue,
			mediumspringgreen,
			mediumturquoise,
			mediumvioletred,
			midnightblue,
			mintcream,
			mistyrose,
			moccasin,
			navajowhite,
			navy,
			oldlace,
			olive,
			olivedrab,
			orange,
			orangered,
			orchid,
			palegoldenrod,
			palegreen,
			paleturquoise,
			palevioletred,
			papayawhip,
			peachpuff,
			peru,
			pink,
			plum,
			powderblue,
			purple,
			red,
			rosybrown,
			royalblue,
			saddlebrown,
			salmon,
			sandybrown,
			seagreen,
			seashell,
			sienna,
			silver,
			skyblue,
			slateblue,
			slategray,
			slategrey,
			snow,
			springgreen,
			steelblue,
			tan,
			teal,
			thistle,
			tomato,
			turquoise,
			violet,
			wheat,
			white,
			whitesmoke,
			yellow,
			yellowgreen
		};
	}
};

#endif //CAMERASTABILIZATION_COLORPALETTE_HPP
