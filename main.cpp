/*
2024/10/10
富山高等専門学校射水キャンパス
1年生教育研修ツアー
author:tharuto
*/

#include<bits/stdc++.h>
#include "json.hpp"
using namespace std;

bool debug_mode = false; //true or false

int size_int[9] = {1,2,4,8,16,32,64,128,256};
int type_int[9] = {0,1,4,7,10,13,16,19,22};

vector<pair<int,int>> memo_point;
vector<pair<int,int>> memo_field;

void print_f(vector<vector<int>> &field){
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			cout << field[i][j] << " ";
		}
	cout << endl;
	}
cout << endl;
}

void post_json(int move_value){
	string post_s = "";
	post_s += "{\"n\":";
	post_s += to_string(move_value);
	post_s += ",\"ops\":[";
	for(int i = 0; i < move_value; i++){
		post_s += "{\"p\":";
		post_s += to_string(memo_field[i].first);
		post_s += ",\"y\":";
		post_s += to_string(memo_point[i].first);
		post_s += ",\"x\":";
		post_s += to_string(memo_point[i].second);
		post_s += ",\"s\":";
		post_s += to_string(memo_field[i].second);
		post_s += "}";
		if(i != move_value-1){
			post_s += ",";
		}
	}
	post_s += "]}";

	ofstream ofs("./post.json");
	ofs << post_s;
	ofs.close();
}


int main(){
	system("python3 get.py");
	ifstream ifs("./data.json");
	string jsonstr;
	
	if(ifs.fail()){//エラー分岐
		cout << "File Open Error" << endl;
		return -1;
	}

	while(getline(ifs, jsonstr)){
		//cout<<"[Read result] "<< endl;
	}

	auto jobj = nlohmann::json::parse(jsonstr);

	int height = jobj["board"]["height"];
	int width = jobj["board"]["width"];

	int move_value = 0;
	int move_select = 0;

	vector<vector<int>> field(height+10, vector<int> (width+10,-1)), answer(height+10, vector<int> (width+10,-1));
	vector<string> field_str(height+10);

	field_str = jobj["board"]["start"];


	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			field[i][j] = field_str[i][j]-48;
		}
	}

	field_str = jobj["board"]["goal"];

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			answer[i][j] = field_str[i][j]-48;
		}
	}

	if(debug_mode){
		cout << "start" << endl;
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				cout << field[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;

		cout << "goal" << endl;
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				cout << answer[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(field[i][j] == answer[i][j]){
				continue;
			}
			else{
				pair<int,int> move_point = {-1,-1};
				queue<pair<int,int>> search_q;
				vector<vector<bool>> flag(height+10, vector<bool> (width+10,true));

				for(int k = 0; k < i; k++){
					for(int l = 0; l < width; l++){
						flag[k][l] = false;
					}
				}

				for(int k = 0; k <= j; k++){
					flag[i][k] = false;
				}
				search_q.push({i,j});
				while(true){
					if(search_q.front().first-1 >= 0){//上方向かつフィールド内のとき
						if(flag[search_q.front().first - 1][search_q.front().second]){
							search_q.push({search_q.front().first - 1, search_q.front().second});
							flag[search_q.front().first - 1][search_q.front().second] = false;
						}
					}

					if(search_q.front().second + 1 < width){//右方向かつフィールド内のとき
						if(flag[search_q.front().first][search_q.front().second + 1]){
							search_q.push({search_q.front().first, search_q.front().second + 1});
							flag[search_q.front().first][search_q.front().second + 1] = false;
						}
					}

					if(search_q.front().first + 1 < height){//下方向かつフィールド内のとき
						if(flag[search_q.front().first + 1][search_q.front().second]){
							search_q.push({search_q.front().first + 1, search_q.front().second});
							flag[search_q.front().first + 1][search_q.front().second] = false;
						}
					}

					if(search_q.front().second - 1 >= 0){//左方向かつフィールド内のとき
						if(flag[search_q.front().first][search_q.front().second - 1]){
							search_q.push({search_q.front().first, search_q.front().second - 1});
							flag[search_q.front().first][search_q.front().second - 1] = false;
						}
					}

					if(field[search_q.front().first][search_q.front().second] == answer[i][j]){
						move_point = {search_q.front().first, search_q.front().second};
						break;
					}

					search_q.pop();
				}

				if(debug_mode){
					cout << "移動回数:" << move_value << endl;
					for(int k = 0; k < height; k++){
						for(int l = 0; l < width; l++){
							if(k == i & l == j){
								cout << "[" << field[k][l] << "]" << " "; 
							}
							else if(k == move_point.first & l == move_point.second){
								cout << "{" << field[k][l] << "}" << " "; 
							}
							else{
								cout << " " << field[k][l] << "  ";
							}
						}
						cout << endl;
					}
					cout << endl;

					//sleep(1);
				}

				//動かす部分
				if(field[i][j] != answer[i][j]){
					move_select++;
				}

				while(field[i][j] != answer[i][j]){

					int movement = -1;
					int itr = -1;
					vector<vector<int>> cut_f(height+10, vector<int> (width+10,-1));
					if(move_point.second != j){
						int step = abs(j - move_point.second);//動かさないといけないマス数
						itr = 8;
						while(size_int[itr] > step){
							itr--;
						}
						if(move_point.second < j){
							movement = 3;
							memo_point.push_back({move_point.first,move_point.second+1});
							for(int cut_i = 0; cut_i < size_int[itr]; cut_i++){//抜く場所を2で置換
								if(field[move_point.first + cut_i][0] == -1){
									break;
								}
								for(int cut_j = 0; cut_j < size_int[itr]; cut_j++){
									if(field[move_point.first + cut_i][move_point.second + 1 + cut_j] == -1){
										break;
									}
									else{
										cut_f[cut_i][cut_j] = field[move_point.first + cut_i][move_point.second + 1 + cut_j];
										field[move_point.first + cut_i][move_point.second + 1 + cut_j] = -2;
									}
								}
							}
							for(int cut_i = height-1; cut_i >= 0; cut_i--){//盤面を右にずらす
								for(int cut_j = width-2; cut_j >= 0; cut_j--){
									if(field[cut_i][cut_j] != -2){
										int itr_j = cut_j;
										while(field[cut_i][itr_j+1] == -2){
											field[cut_i][itr_j+1] = field[cut_i][itr_j];
											field[cut_i][itr_j] = -2;
											itr_j++;
											if(itr_j == 0){
												break;
											}
										}
									}
								}
							}
							//数字を戻す
							int itr_x = 0;
							int itr_y = 0;
							for(int cut_i = 0; cut_i < height; cut_i++){
								for(int cut_j = 0; cut_j < width; cut_j++){
									if(field[cut_i][cut_j] == -2){
										field[cut_i][cut_j] = cut_f[itr_x][itr_y];
										itr_y++;
										if(cut_f[itr_x][itr_y] == -1){
											itr_x++;
											itr_y = 0;
										}
									}
								}
								if(cut_f[itr_x][itr_y] == -1){
									break;
								}
							}
							move_point.second += size_int[itr];
						}

						else{
							movement = 2;
							memo_point.push_back({move_point.first,j});
							for(int cut_i = 0; cut_i < size_int[itr]; cut_i++){//抜く場所を2で置換
								if(field[move_point.first + cut_i][0] == -1){
									break;
								}
								for(int cut_j = 0; cut_j < size_int[itr]; cut_j++){
									if(field[move_point.first + cut_i][j + cut_j] == -1){
										break;
									}
									else{
										cut_f[cut_i][cut_j] = field[move_point.first + cut_i][j + cut_j];
										field[move_point.first + cut_i][j + cut_j] = -2;
									}
								}
							}
							for(int cut_i = 0; cut_i < height; cut_i++){//盤面を左にずらす
								for(int cut_j = 1; cut_j < width; cut_j++){
									if(field[cut_i][cut_j] != -2){
										int itr_j = cut_j;
										while(field[cut_i][itr_j-1] == -2){
											field[cut_i][itr_j-1] = field[cut_i][itr_j];
											field[cut_i][itr_j] = -2;
											itr_j--;
											if(itr_j == 0){
												break;
											}
										}
									}
								}
							}
							//数字を戻す
							int itr_x = 0;
							int itr_y = 0;
							for(int cut_i = 0; cut_i < height; cut_i++){
								for(int cut_j = 0; cut_j < width; cut_j++){
									if(field[cut_i][cut_j] == -2){
										field[cut_i][cut_j] = cut_f[itr_x][itr_y];
										itr_y++;
										if(cut_f[itr_x][itr_y] == -1){
											itr_x++;
											itr_y = 0;
										}
									}
								}
								if(cut_f[itr_x][itr_y] == -1){
									break;
								}
							}
							move_point.second -= size_int[itr];
						}
					}
					else{
						movement = 0;
						memo_point.push_back({i,j});
						int step = abs(i - move_point.first);//動かさないといけないマス数
						itr = 8;
						while(size_int[itr] > step){
							itr--;
						}
						for(int cut_i = 0; cut_i < size_int[itr]; cut_i++){//抜く場所を2で置換
							if(field[i + cut_i][0] == -1){
								break;
							}
							for(int cut_j = 0; cut_j < size_int[itr]; cut_j++){
							if(field[i + cut_i][j + cut_j] == -1){
									break;
								}
								else{
									cut_f[cut_i][cut_j] = field[i + cut_i][j + cut_j];
									field[i + cut_i][j + cut_j] = -2;
								}
							}
						}
						for(int cut_i = 1; cut_i < height; cut_i++){//盤面を上にずらす
							for(int cut_j = 0; cut_j < width; cut_j++){
								if(field[cut_i][cut_j] != -2){
									int itr_i = cut_i;
									while(field[itr_i-1][cut_j] == -2){
										field[itr_i-1][cut_j] = field[itr_i][cut_j];
										field[itr_i][cut_j] = -2;
										itr_i--;
										if(itr_i == 0){
											break;
										}
									}
								}
							}
						}
						//数字を戻す
						int itr_x = 0;
						int itr_y = 0;
						for(int cut_i = 0; cut_i < height; cut_i++){
							for(int cut_j = 0; cut_j < width; cut_j++){
								if(field[cut_i][cut_j] == -2){
									field[cut_i][cut_j] = cut_f[itr_x][itr_y];
									itr_y++;
									if(cut_f[itr_x][itr_y] == -1){
										itr_x++;
										itr_y = 0;
									}
								}
							}
							if(cut_f[itr_x][itr_y] == -1){
								break;
							}
						}
						move_point.first -= size_int[itr];
					}
					move_value++;

					if(debug_mode){
						cout << "移動回数:" << move_value << endl;
						for(int k = 0; k < height; k++){
							for(int l = 0; l < width; l++){
								if(k == i & l == j){
									cout << "[" << field[k][l] << "]" << " "; 
								}
								else if(k == move_point.first & l == move_point.second){
									cout << "{" << field[k][l] << "}" << " "; 
								}
								else{
									cout << " " << field[k][l] << "  ";
								}
							}
							cout << endl;
						}
						cout << endl;

						//sleep(1);
					}
					memo_field.push_back({type_int[itr],movement});
				}
			}
		}

		vector<int> count_1(10,0), count_2(10,0);
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				count_1[field[i][j]]++;
			}
		}

		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				count_2[answer[i][j]]++;
			}
		}

		for(int i = 0; i < 4; i++){
			if(count_1[i] != count_2[i]){
				cout << i << "is NG" << endl;
				break;
			}
		}
	}

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(field[i][j] != answer[i][j]){
				cout << "(" << i << "," << j << ")" << "wrong!!" << endl;
			}
		}
	}

	cout << "選択回数:" << move_select << endl;
	cout << "移動回数:" << move_value << endl;

	post_json(move_value);
	
	system("python3 post.py");
}