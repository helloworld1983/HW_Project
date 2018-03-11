%%%%%%%%%%%%%%%%%%%%%%%%
% 학번 :2013722095    %
% 학과 : 컴퓨터 공학과 %
% 이름 : 최재은        %
%%%%%%%%%%%%%%%%%%%%%%%%%
Count_of_Eigenvector = 4;

Mean_Face = [];
set_mean = zeros(112*92, 1);
%%%%%%%%%%%%%%%%%%% Calculate mean face of each Data set%%%%%%%%%%%%%%%%%%%
for i = 1 : 1 : 6           % folder
    dir_name = strcat('PCA_DB\Training\set_', int2str(i), '\');
    mean = zeros(112*92, 1);
    for j = 1 : 1 : 8       % bmp file
        file_name =  strcat(dir_name, int2str(j), '.bmp');
        disp(file_name);
        data = imread(file_name);
        data = rgb2gray(data);
        data = reshape(data, 112*92, 1);        % 112*92행 X 1열의 형태로 저장
        mean = mean + double(data);
    end
    mean = mean/8;          %mean of set_'i'
    Mean_Face = [Mean_Face mean];
    set_mean = set_mean + mean;
end

set_mean = set_mean/6;  %mean of all face

normalized_vector = [];
for j = 1 : 1 : 6
    vector = double(Mean_Face(:, j)) - set_mean;
    normalized_vector = [normalized_vector vector];
end

message = strcat('Normalized가 완료되었습니다.');
disp(message);

Covariance_matrix = normalized_vector * transpose(normalized_vector);

message = strcat('Covariance_matrix 연산이 완료되었습니다.');
disp(message);

[Eigenvector, Eigenvalue] = eig(Covariance_matrix);

message = strcat('Eigenvector, Eigenvalue 연산이 완료되었습니다.');
disp(message);
   
%아이젠 벡터가 방향이고 벡터가 그 정도를 나타내므로 내림차순 정렬 상위 10개 쓰면 될듯
% eig함수가 아이젠 벡터도 주고 밸류도 줌
% 벡터랑 밸류는 전부다 작은거 -> 큰거 순으로 정렬되어있음.
%%%%% 상위 10개에 대한 eigenvector 추출
% 넣는 거는 강의자료대로 큰것부터 넣음
real_Eigenvector  = [];
for j = 10304: -1: (10304 - Count_of_Eigenvector + 1)
    real_Eigenvector = [real_Eigenvector Eigenvector(:,j)];
end

%%%%%%%%%%%%% input과 비교할 ~x1, ~x2 ....생성 %%%%%%%%%%%%%5
Classifier = [];
for j = 1 : 1 : 6
     feature = transpose(real_Eigenvector) * normalized_vector(:, j);    % get traindata's feature
     Classifier = [Classifier feature];
end  

message = strcat('전체 face들에 대한 Classifier 형성이 완료되었습니다.');
disp(message);

%%%%%%%%%%%% test set recognition %%%%%%%%%%%%%%%%%%%%%%%
dir_name = strcat('PCA_DB\', 'Test\');
for i = 1 : 1 : 12           % test file
    file_name =  strcat(dir_name, int2str(i), '.bmp');
    data = imread(file_name);
    data = rgb2gray(data);
    data = reshape(data, 112*92, 1);        % 1(행) * (112*92)(열)의 형태로 저장
    normalized_vector = double(data) - set_mean;
    feature = transpose(real_Eigenvector) * normalized_vector;
    
     %%%%%%% 유클리드 거리법 %%%%%%%
    distance_set = [];
    for j = 1 : 1 : 6  % 6개의 class에 대해서 
        distance = 0;
        for k = 1: 1: Count_of_Eigenvector   % 각 component 별 차이값의 제곱 합산
            distance = distance + (Classifier(k,j) - feature(k,1))^2;
        end
        distance = sqrt(distance); % 제곱근 
        distance_set = [distance_set distance];
    end
    
    % 최소값 탐색 : min()으로 하려했으나 실수형 양의 정수나 논리형이어야한다 해서 직접 구현
    least = 1;
    for j = 2: 1: 6
        if(distance_set(1, least) > distance_set(1, j))
            least = j;
        end
    end
        
    result = strcat('Recognition of test file[ ',num2str(i),'.bmp] : ', num2str(least));
    disp(result);
end
