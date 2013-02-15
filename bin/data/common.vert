
void main() {
	gl_TexCoord[0] = gl_MultiTexCoord0 * gl_TextureMatrix[0];//テクスチャ座標にテクスチャ変換行列をかける
	gl_Position = ftransform(); //固定機能の変換結果をそのまま使う
} 
